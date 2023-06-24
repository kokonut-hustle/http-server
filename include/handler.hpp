#pragma once

#include "request.hpp"
#include "response.hpp"

class Handler {
public:
    virtual HttpResponse handle(HttpRequest) = 0;
};
