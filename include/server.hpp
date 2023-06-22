#pragma once
#include <iostream>
#include <functional>
#include <sstream>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include "config.hpp"
#include "request.hpp"
#include "response.hpp"
#include "handler.hpp"

const Config::Settings<int> int_settings = {
    // setting,  default value
    {"port", 8080}
};
const Config::Settings<std::string> string_settings = {{"setting1", "val1"}};
const int MAX_BUFFER_SIZE = 4096;

using PathHandlerMap = std::unordered_map<std::string, Handler&>;

class Server {
public:
    Server(PathHandlerMap &&);
    ~Server();

    bool init();
    bool start();
    void write_resp(const HttpResponse &&, int);
private:
    Config::Configuration config;
    int server_socket;
    sockaddr_in server_address;
    PathHandlerMap path_handler_map;
};
