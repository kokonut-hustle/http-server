#pragma once
#include <map>
#include <string>

template<typename T>
using Settings = std::map<std::string, T>;

const Settings<int> int_settings = {
    // setting,  default value
    {"port", 8888},
    {"max_threads", 10},
    {"max_events", 10},
};
const Settings<std::string> string_settings = {{}};

constexpr int MAX_BUFFER_SIZE = 4096;

enum RequestMethod {
    GET,
    POST,
    PUT,
    DELETE
};

constexpr std::array<const char*, 4> METHOD_STR = {
    "GET",
    "POST",
    "PUT",
    "DELETE"
};
