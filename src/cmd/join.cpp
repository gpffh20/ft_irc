#include "../../inc/command.hpp"

void Command::join(Client &client, std::vector<std::string> args) {
	if (args.size() < 2) {
		// 잘못된 명령어 형식 처리
		client.addToSendBuffer(":irc.example.com 461 " + client.getNickname() + " JOIN :Not enough parameters\r\n");
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
	} else {
		std::cout << "Channel exists, joining channel " << channelName << std::endl;
		channel = &server_.getChannels()[channelName];
	}
	
	// 클라이언트를 채널에 추가
	channel->addClient(client);
//	client.addToSendBuffer(
//			":irc.example.com 473 " + client.getNickname() + " " + channelName + " :Cannot join channel (+i)\r\n");
	
	client.addChannel(channel);
	client.addToSendBuffer(
			":" + client.getNickname() + "!" + client.getUsername() + "@" + client.getServername() + " JOIN :"
					+ channelName + "\r\n");
	
	// 채널 주제 전송 (있을 경우)
	if (!channel->getTopic().empty()) {
		client.addToSendBuffer(
				":irc.example.com 332 " + client.getNickname() + " " + channelName + " :" + channel->getTopic()
						+ "\r\n");
	}
	
	// 현재 사용자 리스트 전송
	std::vector <Client *> clientList = channel->getClientList();
	std::string userList;
	for (std::vector<Client *>::iterator it = clientList.begin(); it != clientList.end(); it++) {
		if (!userList.empty()) {
			userList += " ";
		}
		userList += (*it)->getNickname() + " ";
	}
	client.addToSendBuffer(
			":irc.example.com 353 " + client.getNickname() + " = " + channelName + " :" + userList + "\r\n");
	client.addToSendBuffer(
			":irc.example.com 366 " + client.getNickname() + " " + channelName + " :End of /NAMES list.\r\n");
	
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