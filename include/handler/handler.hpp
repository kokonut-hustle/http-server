#pragma once

#include "utils/request.hpp"
#include "utils/response.hpp"

class Handler {
public:
    virtual HttpResponse handle(const HttpRequest &) = 0;
};
