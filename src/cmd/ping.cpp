#include "../../inc/command.hpp"

void Command::ping(Client& client, std::vector<std::string> tokens) {
	if (tokens.size() < 2) {
		client.addToSendBuffer(std::string("409 ") + client.getNickname() + " :No origin specified");
	} else {
		std::string response = "PONG ";
		response += "localhost";
		if (tokens.size() > 2) {
			response += " " + tokens[2];
		}
		client.addToSendBuffer(response);
	}
}
