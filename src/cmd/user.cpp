#include "../../inc/command.hpp"

std::vector<std::string> Command::parseForUSER(std::vector<std::string> args) {
	// Rejoin tokens if ':' is found
	for (size_t i = 1; i < args.size(); ++i) {
		if (!args[i].empty() && args[i][0] == ':') {
			std::string combined = args[i].substr(1);
			for (size_t j = i + 1; j < args.size(); ++j) {
					combined += " " + args[j];
			}
			args.resize(i);
			args.push_back(combined);
			break;
		}
	}
	return args;
}

void Command::user(Client &client, std::vector<std::string> args) {
	args = parseForUSER(args);
	if (!client.getPass()) {
		client.addToSendBuffer(std::string(ERR_NOTREGISTERED) + " :You have not registered\r\n");
		client.setError(true);
		return;
	}
	if (args.size() < 5) {
		client.addToSendBuffer(std::string(ERR_NEEDMOREPARAMS) + "USER :Not enough parameters\r\n");
		return;
	}
	if (client.getIsRegistered()) {
		client.addToSendBuffer(std::string(ERR_ALREADYREGISTRED) + " :You may not register\r\n");
		return;
	}
	client.setUsername(args[1]);
	client.setHostname(args[2]);
	client.setServername(args[3]);
	client.setRealname(args[4]);
	client.setIsRegistered(true);
}