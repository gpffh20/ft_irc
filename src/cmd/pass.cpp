#include "../../inc/command.hpp"

void Command::pass(Client &client, std::vector<std::string> args) {
	if (client.getPass()) {
		client.addToSendBuffer(std::string(ERR_ALREADYREGISTRED) + " :You are already registered");
		return;
	}
	if (args.size() < 2) {
		client.addToSendBuffer(NEEDMOREPARAMS("PASS"));
		return;
	}
	if (args[1] != server_.getPassWord()) {
		client.addToSendBuffer(std::string(ERR_PASSWDMISMATCH) + " :Password incorrect");
		client.setError(true);
		return;
	}
	client.setPass(true);
}