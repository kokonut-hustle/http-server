#pragma once

#include "request.hpp"
#include "response.hpp"

class ParamHandler {
public:
    virtual HttpResponse handle(HttpRequest, const std::unordered_map<std::string, std::string> &) = 0;
};
