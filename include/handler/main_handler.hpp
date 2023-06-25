#pragma once

#include "handler.hpp"

class MainHandler : public Handler {
public:
    HttpResponse handle(const HttpRequest &) override;
};
