#include "../../inc/server.hpp"

void Command::nick(Client& client, std::string token) {
	std::vector<std::string> args = server_.splitBySpace(token);
	client.setNickname(args[1]);
	server_.addNickname(args[1]);
	server_.sendToClient(client.getFd(), ":" + client.getNickname() + " NICK :" + args[1] + "\r\n");

	std::cout << "nickname: " << client.getNickname() << std::endl;
	std::cout << "args[1]: " << args[1] << std::endl;
}