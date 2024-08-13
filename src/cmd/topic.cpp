#include "../../inc/command.hpp"

std::vector<std::string> Command::parseForTOPIC(std::vector<std::string> args) {
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

void Command::topic(Client &client, std::vector<std::string> args) {
	args = parseForTOPIC(args);
//	인자 수 부족
	if (args.size() < 2) {
		client.addToSendBuffer(NEEDMOREPARAMS("TOPIC"));
		return;
	}
	std::string channelName = args[1];
//	채널 없음
	Channel *channel = getChannelByName(channelName);
	if (!channel) {
		client.addToSendBuffer(std::string(ERR_NOSUCHCHANNEL) + " " + client.getNickname() + " " + channelName + " :No such channel\r\n");
		return;
	}
	if (!channel->isClientInChannel(client)) {
		client.addToSendBuffer(std::string(ERR_NOTONCHANNEL) + " " + client.getNickname() + " " + channelName + " :You're not on that channel\r\n");
		return;
	}
//	/topic
	if (args.size() == 2) {
		if (channel->getTopic().empty()) {
			client.addToSendBuffer("331 " + client.getNickname() + " " + channelName + " :No topic is set\r\n");
		} else {
			client.addToSendBuffer("332 " + client.getNickname() + " " + channelName + " :" + channel->getTopic() + "\r\n");
		}
		return ;
	}
	// topic mode 인데 오퍼레이터 권한이 없을 때
	if (channel->getTopicProtected() && !channel->isClientOp(client)) {
		client.addToSendBuffer(std::string(ERR_CHANOPRIVSNEEDED) + " " + client.getNickname() + " " + channelName + " :You're not channel operator\r\n");
		return;
	}
	channel->setTopic(args[2]);
	std::cout << "TOPIC: " << channel->getTopic() << std::endl;
	std::vector<Client *> clientList = channel->getClientList();
	std::string msg =
			":" + client.getNickname() + "!" + client.getHostname() + "@" + client.getServername() + " TOPIC "
					+ channelName + " :" + args[2] + "\r\n";
	for (std::vector<Client *>::iterator it = clientList.begin(); it != clientList.end(); ++it) {
			(*it)->addToSendBuffer(msg);
	}
}
