#include "server.hpp"

Server::Server() {
    config.int_settings = std::move(int_settings);
    config.string_settings = std::move(string_settings);
    Config::load_configuration();
}

Server::~Server() {
    close(server_socket);
}

bool Server::init() {
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1) {
        std::cerr << "Failed to create socket." << std::endl;
        return false;
    }

    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = htonl(INADDR_ANY);
    server_address.sin_port = htons(config.int_settings["port"]);
    return true;
}

bool Server::start() {
    if (bind(server_socket, (struct sockaddr*)&server_address, sizeof(server_address)) < 0) {
        std::cerr << "Failed to bind the socket to the specified address." << std::endl;
        return false;
    }

    if (listen(server_socket, 10) < 0) {
        std::cerr << "Failed to listen on the socket." << std::endl;
        return false;
    }

    std::cout << "Server started on port " << config.int_settings["port"] << std::endl;
    while (true) {
        sockaddr_in client_address{};
        socklen_t client_address_length = sizeof(client_address);
        int client_socket = accept(server_socket, (struct sockaddr*)&client_address, &client_address_length);

        if (client_socket < 0) {
            std::cerr << "Failed to accept the client connection." << std::endl;
            continue;
        }

        char buffer[MAX_BUFFER_SIZE];
        ssize_t bytes_read = read(client_socket, buffer, sizeof(buffer) - 1);

        if (bytes_read < 0) {
            std::cerr << "Failed to read from the client socket." << std::endl;
            close(client_socket);
            continue;
        }

        buffer[bytes_read] = '\0';
        std::string request(buffer);

        std::string response = handle_request(request);

        ssize_t bytes_written = write(client_socket, response.c_str(), response.size());
        if (bytes_written < 0) {
            std::cerr << "Failed to write to the client socket." << std::endl;
        }

        close(client_socket);
    }

    return true;
}
