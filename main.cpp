#include <iostream>

#include "server.hpp"
#include "handler/main_handler.hpp"
#include "param_handler/main_param_handler.hpp"
#include "param_handler/resource_handler.hpp"

int main() {
    MainHandler handler;
    MainParamHandler param_handler;
    ResourceHandler resource_handler;

    PathHandlerMap paths = {
        {{RequestMethod::GET, "/"}, handler},
        // Add more path here...
    };

    ParamHandlerMap param_paths = {
        {{RequestMethod::GET, "/params/{name}"}, param_handler},
        {{RequestMethod::GET, "/resources/{resource_name}"}, resource_handler},
        // Add more routes here...
    };

    Server server(std::move(paths), std::move(param_paths));
    if (!server.init()) return -1;
    if (!server.start()) return -1;

    return 0;
}
