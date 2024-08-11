#include "../../inc/command.hpp"

void Command::join(Client &client, std::vector<std::string> args) {
	if (args.size() < 2) {
		// 잘못된 명령어 형식 처리
		client.addToSendBuffer(NEEDMOREPARAMS("JOIN"));
		return;
	}
	
	std::string channelName = args[1];
	Channel *channel;
	
	if (server_.getChannels().find(channelName) == server_.getChannels().end()) {
		std::cout << "Channel does not exist, creating new channel " << channelName << std::endl;
		// 채널 생성
		Channel newChannel(channelName);
		server_.addChannel(newChannel);
		channel = &server_.getChannels()[channelName];
		// Assign the first user as an operator
		channel->addOp(client);
		client.addToSendBuffer(
				":" + client.getNickname() + "!" + client.getUsername() + "@" + client.getHostname() + " MODE "
						+ channelName + " +o " + client.getNickname() + "\r\n");
	} else {
		std::cout << "Channel exists, joining channel " << channelName << std::endl;
		channel = &server_.getChannels()[channelName];
	}
	
	// 클라이언트를 채널에 추가
	channel->addClient(client);
	client.addChannel(channel);
	client.addToSendBuffer(
			":" + client.getNickname() + "!" + client.getUsername() + "@" + client.getServername() + " JOIN :"
					+ channelName + "\r\n");
	
	// 현재 사용자 리스트 전송
	std::vector < Client * > clientList = channel->getClientList();
	std::string userList;
	for (std::vector<Client *>::iterator it = clientList.begin(); it != clientList.end(); it++) {
		if (channel->isClientOp(**it))
			userList += "@";
		userList += (*it)->getNickname();
		if (it != clientList.end() - 1) {
			userList += " ";
		}
	}
	client.addToSendBuffer("353 " + client.getNickname() + " = " + channelName + " :" + userList + "\r\n");
	client.addToSendBuffer(
			"366 " + client.getNickname() + " " + channelName + " :End of /NAMES list.\r\n");
	
	// 채널의 다른 클라이언트에게 JOIN 메시지 방송
	std::string joinMessage =
			":" + client.getNickname() + "!" + client.getUsername() + "@" + client.getServername() + " JOIN :"
					+ channelName + "\r\n";
	for (std::vector<Client *>::iterator it = clientList.begin(); it != clientList.end(); ++it) {
		if ((*it)->getNickname() != client.getNickname()) {
			(*it)->addToSendBuffer(joinMessage);
		}
	}
}