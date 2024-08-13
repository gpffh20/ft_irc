#include "../../inc/command.hpp"

void Command::nick(Client &client, std::vector<std::string> args) {
	// PASS 등록 안됨
	if (!client.getNick() && !client.getPass()) {
		client.addToSendBuffer(std::string(ERR_NOTREGISTERED) + " :You have not registered\r\n");
		server_.removeClient(client.getFd());
		client.setError(true);
		return;
	}
	// 인자 수 오류
	if (args.size() < 2) {
		client.addToSendBuffer(std::string(ERR_NONICKNAMEGIVEN) + " :No nickname given\r\n");
		return;
	}
	std::string newNickname = args[1];

	// 닉네임 중복
	while (isNicknameExist(newNickname, client.getFd())) {
		if (client.getNick()) {
			client.addToSendBuffer(
				std::string(ERR_NICKNAMEINUSE) + " " + newNickname + " :Nickname is already in use\r\n");
			return;
		} else {
			newNickname += "_";
		}
	}

	// 닉네임 변경 처리
	std::string oldNickname = client.getNickname();
	if (client.getNick()) {
		server_.removeNickname(oldNickname);
	}
	client.setNickname(newNickname);
	server_.addNickname(newNickname);

	// 특정 채널에서의 닉네임 변경 알림
	std::map<std::string, Channel>& channels = server_.getChannels();
	for (std::map<std::string, Channel>::iterator channelIt = channels.begin(); channelIt != channels.end(); ++channelIt) {
		Channel& channel = channelIt->second;
		if (channel.isClientInChannel(client)) {
			std::vector<Client*>& clients = channel.getClientList();
			for (std::vector<Client*>::iterator it_client = clients.begin(); it_client != clients.end(); ++it_client) {
				if ((*it_client)->getNickname() != client.getNickname()) { // 자신을 제외하고 알림
					std::string nickChangeMsg = ":" + oldNickname + "!" + client.getUsername() + "@" + client.getHostname() + " NICK " + newNickname;
					(*it_client)->addToSendBuffer(nickChangeMsg);
				}
			}
		}
	}

	client.addToSendBuffer(":" + oldNickname + " NICK " + newNickname);
	client.setNick(true);
}
