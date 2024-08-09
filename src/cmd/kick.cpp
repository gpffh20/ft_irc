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
	std::string channelName = args[0];
	std::string targetNickname = args[1];
//	std::string reason = args.size() > 2 ? args[2] : "No reason given";
	
	// 채널 없음
	Channel *channel = getChannelByName(channelName);
	if (!channel) {
		client.addToSendBuffer(
				std::string(ERR_NOSUCHCHANNEL) + " " + client.getNickname() + " " + channelName + " :No such channel\r\n");
		return;
	}
	
	// 닉네임 없음
	
	// 내가(명령어를 친 사람) 채널에 없음
	
	// kick 하려는 사람이 채널에 없음
	
	// client가 채널의 operator가 아님
	
	// kick 실행
	
	// 채널에서 client 제거
	
	// client의 채널 목록에서 채널 제거
	
	

	if (!channel->isOp(client.getNickname())) {
		client.addToSendBuffer(std::string(ERR_CHANOPRIVSNEEDED) + " " + client.getNickname() + " " + channelName
									   + " :You're not channel operator\r\n");
		return;
	}
	
	Client *targetClient = channel->getClientByNickname(targetNickname);
	if (!targetClient) {
		client.addToSendBuffer(
				std::string(ERR_USERNOTINCHANNEL) + " " + client.getNickname() + " " + targetNickname + " " + channelName
						+ " :They aren't on that channel\r\n");
		return;
	}
	
//	// Send KICK message to the channel and the target client
//	std::string kickMessage =
//			":" + client.getNickname() + " KICK " + channelName + " " + targetNickname + " :" + reason + "\r\n";
//	channel->sendToChannel(kickMessage);
//	channel->broadcast(kickMessage);
//	targetClient->addToSendBuffer(kickMessage);
//
//	// Remove the client from the channel
//	channel->removeClient(targetClient);
//	targetClient->removeChannel(channelName);
}