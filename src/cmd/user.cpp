#include "../../inc/server.hpp"

void Command::user(Client& client, std::string token) {
	std::vector<std::string> args = server_.splitBySpace(token);
	if (args.size() < 5) {
		server_.sendToClient(client.getFd(), "461 USER :Not enough parameters\r\n");
		return;
	}
	client.setUsername(args[1]);
	client.setRealname(args[4]);
	client.setIsRegistered(true);
	server_.sendToClient(client.getFd(), ":" + client.getNickname() + " USER " + args[1] + " 0 * :" + args[4] + "\r\n");
}