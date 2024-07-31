#include "../../inc/server.hpp"

void Command::nick(Client& client, std::vector<std::string> args) {
	client.setNickname(args[1]);
	server_.addNickname(args[1]);
	server_.sendToClient(client.getFd(), ":" + client.getNickname() + " NICK :" + args[1] + "\r\n");
}