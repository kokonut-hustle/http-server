#include "server.hpp"
#include "utils/request.hpp"
#include "utils/response.hpp"
#include "handler/error_page_handler.hpp"
#include "utils/log.hpp"

Server::Server(PathHandlerMap &&_path_handler_map, ParamHandlerMap &&_param_handler_map)
    : path_handler_map(_path_handler_map),
     param_handler_map(_param_handler_map) {
    config.conf_s.int_settings = std::move(int_settings);
    config.conf_s.string_settings = std::move(string_settings);
    config.load_configuration();
}

Server::~Server() {
    close(server_socket);
    close(epollfd);

    for (std::thread& thread : threads) {
        thread.join();
    }
}

bool Server::init() {
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1) {
        LogErr("Failed to create socket.");
        // std::cerr << "Failed to create socket." << std::endl;
        return false;
    }

    int enable = 1;
    if (setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) == -1) {
        // std::cerr << "Failed to set socket options" << std::endl;
        close(server_socket);
        return false;
    }

    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = htonl(INADDR_ANY);
    server_address.sin_port = htons(config.conf_s.int_settings["port"]);
    return true;
}

bool Server::start() {
    if (bind(server_socket, (struct sockaddr*)&server_address, sizeof(server_address)) < 0) {
        LogErr("Failed to bind the socket to the specified address.");
        std::cerr << "Failed to bind the socket to the specified address." << std::endl;
        close(server_socket);
        return false;
    }

    if (listen(server_socket, 10) < 0) {
        LogErr("Failed to listen on the socket.");
        std::cerr << "Failed to listen on the socket." << std::endl;
        close(server_socket);
        return false;
    }

    LogInfo("Server started on port " + std::to_string(config.conf_s.int_settings["port"]));
    std::cout << "Server started on port " << config.conf_s.int_settings["port"] << std::endl;

    for (int i = 0; i < MAX_THREADS; ++i) {
        std::thread thread([this]() {
            this->worker_thread();
        });
        threads.push_back(std::move(thread));
    }

    int epollfd = epoll_create1(0);
    if (epollfd == -1) {
        LogErr("Failed to create epoll instance");
        // std::cerr << "Failed to create epoll instance" << std::endl;
        close(server_socket);
        return false;
    }

    struct epoll_event event{};
    event.events = EPOLLIN;
    event.data.fd = server_socket;

    if (epoll_ctl(epollfd, EPOLL_CTL_ADD, server_socket, &event) == -1) {
        LogErr("Failed to add server socket to epoll");
        // std::cerr << "Failed to add server socket to epoll" << std::endl;
        close(server_socket);
        close(epollfd);
        return false;
    }

    std::vector<struct epoll_event> events(MAX_EVENTS);

    while (true) {
        int num_events = epoll_wait(epollfd, events.data(), MAX_EVENTS, -1);
        if (num_events == -1) {
            if (errno == EINTR) {
                continue;
            }
            // std::cerr << "Error in epoll_wait" << std::endl;
            break;
        }

        for (int i = 0; i < num_events; ++i) {
            if (events[i].data.fd == server_socket) {
                sockaddr_in client_address{};
                socklen_t client_address_length = sizeof(client_address);
                int client_socket = accept(server_socket, (struct sockaddr*)&client_address, &client_address_length);

                if (client_socket < 0) {
                    LogErr("Failed to accept the client connection.");
                    // std::cerr << "Failed to accept the client connection." << std::endl;
                    continue;
                }

                struct timeval timeout;
                timeout.tv_sec = 5;
                timeout.tv_usec = 0;

                // Set the timeout for the client socket
                if (setsockopt(client_socket, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout)) == -1) {
                    LogErr("Error setting socket timeout");
                    // std::cerr << "Error setting socket timeout" << std::endl;
                    close(client_socket);
                    continue;
                }

                {
                    std::unique_lock<std::mutex> lock(mtx);
                    client_queue.push(client_socket);
                }
                cv.notify_one();
            }
        }
    }

    return true;
}

void Server::worker_thread() {
    while (true) {
        int client_socket = -1;

        {
            std::unique_lock<std::mutex> lock(mtx);
            cv.wait(lock, [this] { return !this->client_queue.empty(); });

            client_socket = client_queue.front();
            client_queue.pop();
        }

        // Handle the connection in a separate function
        handle_client_connection(client_socket);
    }

}

void Server::handle_client_connection(int client_socket) {
    char buffer[MAX_BUFFER_SIZE];
    ssize_t bytes_read = read(client_socket, buffer, sizeof(buffer) - 1);

    if (bytes_read <= 0) {
        if (bytes_read == 0) {
            LogInfo("Client disconnected");
            // std::cout << "Client disconnected" << std::endl;
        } else { // Maybe client socket is timeout
            LogInfo("Cannot receiving data from client: " + std::to_string(errno));
            // std::cerr << "Cannot receiving data from client: " << errno << std::endl;
        }
        close(client_socket);
        return;
    }

    buffer[bytes_read] = '\0';
    std::string request(buffer);

    HttpRequest serv(request);
    if (!serv.parse()) {
        LogWarn("Unable to parse request: " + request);
        write_resp(std::move(ErrorPageHandler::handle()), client_socket);
    }

    std::unordered_map<std::string, std::string> params;
    if (auto it = path_handler_map.find({serv.get_method(), serv.get_path()});
        it != path_handler_map.end()) {
        Handler &handler = it->second;
        write_resp(handler.handle(serv), client_socket);
    } else if (auto it = search_param_req(serv.get_method(), serv.get_path(), params);
        it != param_handler_map.end()) {
        ParamHandler &handler = it->second;
        write_resp(handler.handle(serv, params), client_socket);
    } else {
        LogInfo("No handler found for: " + serv.get_path());
        // std::cout << "No handler found for: " << serv.get_path() << std::endl;
        write_resp(std::move(ErrorPageHandler::handle()), client_socket);
    }

    close(client_socket);
}

ParamHandlerMap::iterator Server::search_param_req(const RequestMethod &method,
    const std::string &path,
    std::unordered_map<std::string, std::string> &params) {
    // Search for a matching route
    for (auto it = param_handler_map.begin(); it != param_handler_map.end(); ++it) {
        auto element = *it;
        if (element.first.first != method)
            continue;

        std::string pattern = element.first.second;

        // Split the pattern and path into segments
        size_t pattern_pos = 0;
        size_t path_pos = 0;
        bool match = true;

        while (pattern_pos < pattern.length() && path_pos < path.length()) {
            if (pattern[pattern_pos] == '{') {
                // Capture the parameter value until '/'
                size_t closing_brace_pos = pattern.find('}', pattern_pos);
                if (closing_brace_pos == std::string::npos) {
                    match = false;
                    break;
                }
                std::string param_name = pattern.substr(pattern_pos + 1, closing_brace_pos - pattern_pos - 1);

                size_t next_slash_pos = path.find('/', path_pos);
                if (next_slash_pos == std::string::npos) {
                    next_slash_pos = path.length();
                }
                std::string param_value = path.substr(path_pos, next_slash_pos - path_pos);
                params[param_name] = param_value;

                pattern_pos = closing_brace_pos + 1;
                path_pos = next_slash_pos;
            } else if (pattern[pattern_pos] != path[path_pos]) {
                match = false;
                break;
            } else {
                pattern_pos++;
                path_pos++;
            }
        }

        if (match && pattern_pos == pattern.length() && path_pos == path.length()) {
            return it;
        }
    }
    return param_handler_map.end();
}

void Server::write_resp(const HttpResponse &&resp, int client_socket) {
    std::string response = resp.to_string();

    ssize_t bytes_written = write(client_socket, response.c_str(), response.size());
    if (bytes_written < 0) {
        LogErr("Failed to write to the client socket");
        // std::cerr << "Failed to write to the client socket." << std::endl;
    }
}
