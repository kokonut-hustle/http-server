#pragma once

#include "utils/request.hpp"
#include "utils/response.hpp"

class ParamHandler {
public:
    virtual HttpResponse handle(const HttpRequest &, const std::unordered_map<std::string, std::string> &) = 0;
};
