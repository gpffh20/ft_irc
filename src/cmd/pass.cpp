#include "../../inc/server.hpp"

void Command::pass(Client& client, std::string token) {
	std::vector<std::string> args = server_.splitBySpace(token);
    if (args.size() < 2) {
        server_.sendToClient(client.getFd(), "461 PASS :Not enough parameters\r\n");
        return;
    }
	std::string password = args[1];
	std::cout << "password: " << password << std::endl;
	std::cout << "server password: " << server_.getPassWord() << std::endl;
    if (password != server_.getPassWord()) {
		std::cout << "password incorrect" << std::endl;
        server_.sendToClient(client.getFd(), "464 :Password incorrect\r\n");
        return;
    }
	client.setIsRegistered(true);
}