#pragma once

#include "request.hpp"
#include "response.hpp"

class ParamHandler {
public:
    ParamHandler() : keep_alive(true) {}
    virtual HttpResponse handle(HttpRequest, const std::unordered_map<std::string, std::string> &) = 0;
    bool keep_alive;
};
