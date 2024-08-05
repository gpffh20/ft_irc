#include "../../inc/command.hpp"

void Command::join(Client& client, std::vector<std::string> args) {
    (void)client;
    std::cout << args[1] << std::endl;
    if (server_.getChannels().find(args[1]) == server_.getChannels().end()) {
        std::cout << "Channel does not exist" << std::endl;
        //create channel
        Channel newChannel(args[1]);
        // newChannel.addClient(client);
        // server_.addChannel(newChannel);
        std::cout << "Created channel " << args[1] << std::endl;
    } else {
        // server_.getChannels()[args[1]].addClient(client);
        std::cout << "Joined channel " << args[1] << std::endl;
    }
}