#include "../../inc/command.hpp"

std::vector<std::string> Command::parseForQuit(std::vector<std::string> args) {
	for (size_t i = 2; i < args.size(); ++i) {
		args[1] += " " + args[i];
	}
	args.resize(2);
	return args;
}

void Command::quit(Client& client, std::vector<std::string> args) {
	(void) client;
	std::string quitMessage;
	if (args.size() < 2) {
		quitMessage = ":" + client.getNickname() + "!" + client.getHostname() + "@" + client.getServername() + " QUIT :Client Quit\r\n";
	} else {
		args = parseForQuit(args);
		quitMessage = ":" + client.getNickname() + "!" + client.getHostname() + "@" + client.getServername() + " QUIT " + args[1] + "\r\n";
	}
	client.addToSendBuffer(quitMessage);
	
}
