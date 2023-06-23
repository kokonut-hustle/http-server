#include <iostream>

#include "server.hpp"
#include "main_handler.hpp"
#include "main_param_handler.hpp"

int main() {
    MainHandler handler;
    MainParamHandler param_handler;

    PathHandlerMap paths = {
        {{RequestMethod::GET, "/"}, handler},
        // Add more path here...
    };

    ParamHandlerMap param_paths = {
        {{RequestMethod::GET, "/params/{name}"}, param_handler},
        // Add more routes here...
    };

    Server server(std::move(paths), std::move(param_paths));
    if (!server.init()) return -1;
    if (!server.start()) return -1;

    return 0;
}
