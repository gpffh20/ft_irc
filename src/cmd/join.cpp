#include "../../inc/command.hpp"

void Command::join(Client& client, std::vector<std::string> args) {
    (void)client;
    std::cout << args[1] << std::endl;
    Channel newChannel = Channel(args[1]);
    if (server_.getChannels().find(args[1]) == server_.getChannels().end()) {
        server_.getChannels().insert(std::pair<std::string, Channel>(args[1], newChannel));
    }
}