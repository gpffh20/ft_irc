//#include "../../inc/command.hpp"
//
//void Command::invite(Client& client, std::vector<std::string> args) {
//	if (args.size() < 3) {
//		client.addToSendBuffer(NEEDMOREPARAMS("INVITE"));
//		return;
//	}
//	std::string nickname = args[1];
//	std::string channel_name = args[2];
//	std::map<std::string, Channel>::iterator it = server_.getChannels().find(channel_name);
//	if (it == server_.getChannels().end()) {
//		client.addToSendBuffer(ERR_NOSUCHCHANNEL + client.getNickname() + " " + channel_name + " :No such channel\r\n");
//		return;
//	}
//	if (it->second.getUsers().find(client.getNickname()) == it->second.getUsers().end()) {
//		client.addToSendBuffer(ERR_NOTONCHANNEL + client.getNickname() + " " + channel_name + " :You're not on that channel\r\n");
//		return;
//	}
//	std::map<std::string, Client>::iterator it2 = server_.getClients().find(nickname);
//	if (it2 == server_.getClients().end()) {
//		client.addToSendBuffer(ERR_NOSUCHNICK + client.getNickname() + " " + nickname + " :No such nick/channel\r\n");
//		return;
//	}
//	it2->second.addToSendBuffer(":" + client.getNickname() + " INVITE " + nickname + " " + channel_name + "\r\n");
//	it2->second.addToSendBuffer(":" + client.getNickname() + " INVITE " + nickname + " " + channel_name + "\r\n");
//}