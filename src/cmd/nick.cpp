#include "../../inc/command.hpp"

void Command::nick(Client& client, std::vector<std::string> args) {
	if (client.getIsRegistered() == false) {
		client.addToSendBuffer(std::string(ERR_NOTREGISTERED) + " :You have not registered\r\n");
		return;
	}
	
	if (args.size() < 2) {
		client.addToSendBuffer(std::string(ERR_NONICKNAMEGIVEN) + " :No nickname given\r\n");
		return;
	}
	
	if (isNicknameExist(args[1])) {
		client.addToSendBuffer(std::string(ERR_NICKNAMEINUSE) + " " + args[1] + " :Nickname is already in use\r\n");
		return;
	}
	client.setNickname(args[1]);
	server_.addNickname(args[1]);
	
//	server_.sendToClient(client.getFd(), ":" + client.getNickname() + " NICK :" + args[1] + "\r\n");
}