#pragma once

#include "param_handler.hpp"

class ResourceHandler : public ParamHandler {
    HttpResponse handle(const HttpRequest &, const std::unordered_map<std::string, std::string> &) override;
    std::string read_file_content(const std::string &);
};
