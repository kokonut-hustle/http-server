#pragma once

#include  "handler.hpp"

class ErrorPageHandler {
public:
    static HttpResponse &handle();
private:
    static HttpResponse resp;
};
