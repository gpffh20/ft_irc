#include "IRCServer.hpp"
#include <iostream>

int main() {
    try {
        IRCServer server(6667);
        server.run();
    } catch (const std::runtime_error& e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }
    return 0;
}
