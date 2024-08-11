#include "../../inc/command.hpp"

std::vector<std::string> Command::parseForKICK(std::vector<std::string> args) {
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

void Command::kick(Client &client, std::vector<std::string> args) {
	// KICK <channel> <nickname> [<reason>]
	// 인자 수 부족
	if (args.size() < 3) {
		client.addToSendBuffer(NEEDMOREPARAMS("KICK"));
		return;
	}
	args = parseForKICK(args);
	std::string channelName = args[1];
	std::string targetNickname = args[2];
	std::string reason = args.size() > 2 ? args[3] : "No reason given";
	
	// 닉네임 없음
	std::vector<std::string> &nicknames = server_.getNicknames();
	if (std::find(nicknames.begin(), nicknames.end(), targetNickname) == nicknames.end()) {
		client.addToSendBuffer(
				std::string(ERR_NOSUCHNICK) + " " + client.getNickname() + " " + targetNickname + " :No such nick\r\n");
		return;
	}
	
	// 채널 없음
	Channel *channel = getChannelByName(channelName);
	if (!channel) {
		client.addToSendBuffer(
				std::string(ERR_NOSUCHCHANNEL) + " " + client.getNickname() + " " + channelName
						+ " :No such channel\r\n");
		return;
	}
	
	// 내가(명령어를 친 사람) 채널에 없음
	if (!channel->isClientInChannel(client)) {
		client.addToSendBuffer(
				std::string(ERR_NOTONCHANNEL) + " " + client.getNickname() + " " + channelName
						+ " :You're not on that channel\r\n");
		return;
	}
	
	// kick 하려는 사람이 채널에 없음
	Client *target = channel->getClientByNickname(targetNickname);
	if (!channel->isClientInChannel(*target)) {
		client.addToSendBuffer(
				std::string(ERR_USERNOTINCHANNEL) + " " + client.getNickname() + " " + targetNickname + " "
						+ channelName + " :They aren't on that channel\r\n");
		return;
	}
	
	// client가 채널의 operator가 아님
	if (!channel->isClientOp(client)) {
		client.addToSendBuffer(std::string(ERR_CHANOPRIVSNEEDED) + " " + client.getNickname() + " " + channelName
									   + " :You're not channel operator\r\n");
		return;
	}
	
	// kick 실행
	client.addToSendBuffer(
			":" + client.getNickname() + "!" + client.getHostname() + "@" + client.getServername() + " KICK "
					+ channelName + targetNickname + " :" + reason + "\r\n");
	
	
	// 채널에서 client 제거
	channel->removeClient(*target);
	
	// client의 채널 목록에서 채널 제거
	target->removeChannel(channel);
}