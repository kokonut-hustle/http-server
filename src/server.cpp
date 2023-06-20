#include "server.hpp"

Server::Server() {
    config.int_settings = std::move(int_settings);
    config.string_settings = std::move(string_settings);
    Config::load_configuration();
}

Server::~Server() {
    close(serverSocket);
}

bool Server::init() {
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == -1) {
        std::cerr << "Failed to create socket." << std::endl;
        return false;
    }

    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = htonl(INADDR_ANY);
    serverAddress.sin_port = htons(config.int_settings["port"]);
    return true;
}

bool Server::start() {
    if (bind(serverSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) < 0) {
        std::cerr << "Failed to bind the socket to the specified address." << std::endl;
        return false;
    }

    if (listen(serverSocket, 10) < 0) {
        std::cerr << "Failed to listen on the socket." << std::endl;
        return false;
    }

    std::cout << "Server started on port " << config.int_settings["port"] << std::endl;
    while (true) {
        sockaddr_in clientAddress{};
        socklen_t clientAddressLength = sizeof(clientAddress);
        int clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddress, &clientAddressLength);

        if (clientSocket < 0) {
            std::cerr << "Failed to accept the client connection." << std::endl;
            continue;
        }

        char buffer[MAX_BUFFER_SIZE];
        ssize_t bytesRead = read(clientSocket, buffer, sizeof(buffer) - 1);

        if (bytesRead < 0) {
            std::cerr << "Failed to read from the client socket." << std::endl;
            close(clientSocket);
            continue;
        }

        buffer[bytesRead] = '\0';
        std::string request(buffer);

        std::string response = handleRequest(request);

        ssize_t bytesWritten = write(clientSocket, response.c_str(), response.size());
        if (bytesWritten < 0) {
            std::cerr << "Failed to write to the client socket." << std::endl;
        }

        close(clientSocket);
    }

    return true;
}
