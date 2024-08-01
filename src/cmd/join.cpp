#include "../../inc/command.hpp"

void Command::join(Client& client, std::vector<std::string> args) {
    (void)client;
    std::cout << args[1] << std::endl;
    Channel newChannel = Channel(args[1]);
}