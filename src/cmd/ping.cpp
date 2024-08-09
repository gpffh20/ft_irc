#include "../../inc/command.hpp"

void Command::ping(Client& client, std::vector<std::string> tokens) {
		if (tokens.size() == 1) {
				client.addToSendBuffer(std::string(ERR_NOORIGIN) + " " + client.getNickname() + " :No origin specified\r\n");
		} else if (tokens.size() == 2) {
				// 여기서 PONG 출력문이 아니라 PONG 함수를 호출하게 해야 함
				client.addToSendBuffer("PONG :" + tokens[1] + "\r\n");
		} else {
				client.addToSendBuffer("PONG :" + tokens[1] + " " + tokens[2] + "\r\n");
		}
}
