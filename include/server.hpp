#pragma once
#include <iostream>
#include <sstream>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include "config.hpp"
#include "request.hpp"
#include "response.hpp"
#include "handler.hpp"
#include "param_handler.hpp"

const Config::Settings<int> int_settings = {
    // setting,  default value
    {"port", 8080}
};
const Config::Settings<std::string> string_settings = {{"setting1", "val1"}};
const int MAX_BUFFER_SIZE = 4096;

struct PairHash {
    template <typename T1, typename T2>
    std::size_t operator()(const std::pair<T1, T2>& pair) const {
        auto hash1 = std::hash<T1>{}(pair.first);
        auto hash2 = std::hash<T2>{}(pair.second);
        return hash1 ^ hash2;
    }
};

using req_id = std::pair<RequestMethod, std::string>;
using PathHandlerMap = std::unordered_map<req_id, Handler&, PairHash>;
using ParamHandlerMap = std::unordered_map<req_id, ParamHandler&, PairHash>;

class Server {
public:
    Server(PathHandlerMap &&, ParamHandlerMap &&);
    ~Server();

    bool init();
    bool start();
private:
    ParamHandlerMap::iterator search_param_req(const RequestMethod &,
        const std::string &,
        std::unordered_map<std::string, std::string> &);
    void write_resp(const HttpResponse &&, int);

    Config::Configuration config;
    int server_socket;
    sockaddr_in server_address;
    PathHandlerMap path_handler_map;
    ParamHandlerMap param_handler_map;
};
