#pragma once
#include <iostream>
#include <sstream>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include "config.hpp"

const Config::Settings<int> int_settings = {
    // setting,  default value
    {"port", 8080}
};
const Config::Settings<std::string> string_settings = {{"setting1", "val1"}};
const int MAX_BUFFER_SIZE = 4096;
const std::map<int, std::string> statusCodes = {
    {200, "OK"},
    {400, "Bad Request"},
    {404, "Not Found"},
    {500, "Internal Server Error"}
};

class Server {
public:
    Server();
    ~Server();

    bool init();
    bool start();
private:
    Config::Configuration config;
    int serverSocket;
    sockaddr_in serverAddress;

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
        return generateResponse(200, statusCodes.at(200), responseContent);
    }
};
