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
const std::map<int, std::string> status_codes = {
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
    int server_socket;
    sockaddr_in server_address;

    std::string generate_response(int status_code, const std::string& status_message, const std::string& content) {
        std::ostringstream oss;
        oss << "HTTP/1.1 " << status_code << " " << status_message << "\r\n";
        oss << "Content-Length: " << content.size() << "\r\n";
        oss << "Content-Type: text/plain\r\n";
        oss << "Connection: close\r\n";
        oss << "\r\n";
        oss << content;
        return oss.str();
    }

    std::string handle_request(const std::string& request) {
        // Here, you can implement your own logic to handle different HTTP methods and routes
        // For simplicity, we'll just return a plain response for any request
        std::string response_content = "Hello, World!";
        return generate_response(200, status_codes.at(200), response_content);
    }
};
