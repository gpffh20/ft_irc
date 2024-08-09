#include "../../inc/command.hpp"

void Command::ping(Client& client, std::vector<std::string> tokens) {
		if (tokens.size() == 1) {
			client.addToSendBuffer(std::string(ERR_NOORIGIN) + " " + client.getNickname() + " :No origin specified\r\n");
		} else if (tokens.size() == 2) {
			client.addToSendBuffer("PONG " + tokens[1] + "\r\n");
		} else {
			client.addToSendBuffer("PONG " + tokens[1] + " " + tokens[2] + "\r\n");
		}
}
