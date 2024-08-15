#include "../../inc/command.hpp"

void Command::join(Client &client, std::vector<std::string> args) {
	if (args.size() < 2) {
		// 잘못된 명령어 형식 처리
		client.addToSendBuffer(NEEDMOREPARAMS("JOIN"));
		return;
	}

	std::string channelName = args[1];
	// 사용자가 제공한 비밀번호를 저장
	std::string keyProvided = args.size() > 2 ? args[2] : "";
	Channel *channel;

	if (server_.getChannels().find(channelName) == server_.getChannels().end()) {
		std::cout << "Channel does not exist, creating new channel " << channelName << std::endl;
		// 채널 생성
		Channel newChannel(channelName);
		server_.addChannel(newChannel);
		channel = &server_.getChannels()[channelName];
		// 첫 번째 유저를 운영자로 지정
		channel->addOp(client);
		client.addToSendBuffer(
				":" + client.getNickname() + "!" + client.getUsername() + "@" + client.getHostname() + " MODE "
						+ channelName + " +o " + client.getNickname());
	} else {
		std::cout << "Channel exists, joining channel " << channelName << std::endl;
		channel = &server_.getChannels()[channelName];
		// 초대 전용 모드 확인 및 초대받지 않은 클라이언트 거부
		if (channel->getInviteOnly() && !channel->isClientInvited(client)) {
			client.addToSendBuffer(std::string(ERR_INVITEONLYCHAN) + " " + client.getNickname() + " " + channelName + " :Cannot join channel (invite only)");
			return;
		}

		// 비밀번호 모드 확인 및 비밀번호가 맞는지 확인
		if (!channel->getChannelKey().empty() && channel->getChannelKey() != keyProvided) {
			client.addToSendBuffer(std::string(ERR_BADCHANNELKEY) + " " + client.getNickname() + " " + channelName + " :Cannot join channel (Incorrect channel key)");
			return;
		}

		// 클라이언트 수 제한 확인
		if (channel->getMaxClient() > 0 && channel->getClientList().size() >= (unsigned long)(channel->getMaxClient())) {
			client.addToSendBuffer(std::string(ERR_CHANNELISFULL) + " " + client.getNickname() + " " + channelName + " :Cannot join channel (channel is full)");
			return;
		}
	}
	
	// 클라이언트를 채널에 추가
	channel->addClient(client);
	client.addChannel(channel);
	client.addToSendBuffer(
			":" + client.getNickname() + "!" + client.getUsername() + "@" + client.getServername() + " JOIN :"
					+ channelName);

	if (!channel->getTopic().empty()) {
		client.addToSendBuffer("332 " + client.getNickname() + " " + channelName + " :" + channel->getTopic());
	}
	
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
	client.addToSendBuffer("353 " + client.getNickname() + " = " + channelName + " :" + userList);
	client.addToSendBuffer(
			"366 " + client.getNickname() + " " + channelName + " :End of /NAMES list.");
	
	// 채널의 다른 클라이언트에게 JOIN 메시지 방송
	std::string joinMessage =
			":" + client.getNickname() + "!" + client.getUsername() + "@" + client.getServername() + " JOIN :"
					+ channelName;
	for (std::vector<Client *>::iterator it = clientList.begin(); it != clientList.end(); ++it) {
		if ((*it)->getNickname() != client.getNickname()) {
			if (Server::getClients().find((*it)->getFd()) != Server::getClients().end()) {
			(*it)->addToSendBuffer(joinMessage);
			}
		}
	}
}