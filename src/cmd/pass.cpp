#include "../../inc/command.hpp"

void Command::pass(Client &client, std::vector<std::string> args) {
	std::cout << "client.servername: " << client.getServername() << std::endl;
	if (args.size() < 2) {
		client.addToSendBuffer(NEEDMOREPARAMS("PASS"));
		return;
	}
	if (client.getPass()) {
		client.addToSendBuffer(std::string(ERR_ALREADYREGISTRED) + " :You are already registered\r\n");
		return;
	}
	if (args[1] != server_.getPassWord()) {
		client.addToSendBuffer(std::string(ERR_PASSWDMISMATCH) + " :Password incorrect\r\n");
		client.setError(true);
		return;
	}
	client.setPass(true);
}