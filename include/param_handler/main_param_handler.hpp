#pragma once

#include "param_handler.hpp"

class MainParamHandler : public ParamHandler {
    HttpResponse handle(const HttpRequest &, const std::unordered_map<std::string, std::string> &) override;
};
