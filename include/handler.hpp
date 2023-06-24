#pragma once

#include "request.hpp"
#include "response.hpp"

class Handler {
public:
    Handler() : keep_alive(true){};
    virtual HttpResponse handle(HttpRequest) = 0;
    bool keep_alive;
};
