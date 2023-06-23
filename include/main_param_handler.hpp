#pragma once

#include "param_handler.hpp"

class MainParamHandler : public ParamHandler {
    HttpResponse handle(HttpRequest, const std::unordered_map<std::string, std::string> &) override;
};
