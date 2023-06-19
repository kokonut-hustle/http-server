#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <map>
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>

const int MAX_BUFFER_SIZE = 4096;
const int DEFAULT_PORT = 8080;

std::map<int, std::string> statusCodes = {
    {200, "OK"},
    {400, "Bad Request"},
    {404, "Not Found"},
    {500, "Internal Server Error"}
};

std::string generateResponse(int statusCode, const std::string& statusMessage, const std::string& content) {
    std::ostringstream oss;
    oss << "HTTP/1.1 " << statusCode << " " << statusMessage << "\r\n";
    oss << "Content-Length: " << content.size() << "\r\n";
    oss << "Content-Type: text/plain\r\n";
    oss << "Connection: close\r\n";
    oss << "\r\n";
    oss << content;
    return oss.str();
}

std::string handleRequest(const std::string& request) {
    // Here, you can implement your own logic to handle different HTTP methods and routes
    // For simplicity, we'll just return a plain response for any request
    std::string responseContent = "Hello, World!";
    return generateResponse(200, statusCodes[200], responseContent);
}

int main() {
    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == -1) {
        std::cerr << "Failed to create socket." << std::endl;
        return -1;
    }

    int port = DEFAULT_PORT;

    sockaddr_in serverAddress{};
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = htonl(INADDR_ANY);
    serverAddress.sin_port = htons(port);

    if (bind(serverSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) < 0) {
        std::cerr << "Failed to bind the socket to the specified address." << std::endl;
        perror("Failed to bind the socket to the specified address");
        return -1;
    }

    if (listen(serverSocket, 10) < 0) {
        std::cerr << "Failed to listen on the socket." << std::endl;
        return -1;
    }

    std::cout << "Server started on port " << port << std::endl;

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

    close(serverSocket);
    return 0;
}
