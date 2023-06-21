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
};
