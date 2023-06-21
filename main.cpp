#include <iostream>

#include "server.hpp"

int main() {
    Server server;
    if (!server.init()) return -1;
    if (!server.start()) return -1;

    return 0;
}
