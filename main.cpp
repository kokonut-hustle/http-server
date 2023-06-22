#include <iostream>

#include "server.hpp"
#include "main_handler.hpp"

int main() {
    MainHandler handler;
    Server server(PathHandlerMap({{std::string("GET /"), handler}}));
    if (!server.init()) return -1;
    if (!server.start()) return -1;

    return 0;
}
