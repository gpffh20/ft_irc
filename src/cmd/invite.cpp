#include "../../inc/command.hpp"

void Command::invite(Client& client, std::vector<std::string> args) {
	if (args.size() < 3) {
		client.addToSendBuffer(NEEDMOREPARAMS("INVITE"));
		return;
	}
	std::string nickname = args[1];
	std::string channel_name = args[2];
	std::map<std::string, Channel>::iterator it = server_.getChannels().find(channel_name);
	if (it == server_.getChannels().end()) {
		client.addToSendBuffer(ERR_NOSUCHCHANNEL + client.getNickname() + " " + channel_name + " :No such channel");
		return;
	}
	// channel안에 해당 client가 없으면 error
	std::vector<Client *> &clients = it->second.getClientList();
	bool client_found = false;
	for (std::vector<Client *>::iterator it_client = clients.begin(); it_client != clients.end(); ++it_client) {
		if ((*it_client)->getNickname() == client.getNickname()) {
			client_found = true;
			break;
		}
	}
	if (!client_found) {
		client.addToSendBuffer(
				ERR_NOTONCHANNEL + client.getNickname() + " " + channel_name + " :You're not on that channel");
		return;
	}

	// 대상 클라이언트 찾기
	Client* target_client = NULL;
	std::map<int, Client>::iterator it_client;
	for (it_client = server_.getClients().begin(); it_client != server_.getClients().end(); ++it_client) {
		if (it_client->second.getNickname() == nickname) {
			target_client = &it_client->second;
			break;
		}
	}

	if (target_client == NULL) {
		client.addToSendBuffer(std::string(ERR_NOSUCHNICK) + " " + client.getNickname() + " " + nickname + " :No such nick/channel");
		return;
	}

	// 초대 상태 업데이트
	if (!it->second.isClientInvited(*target_client)) {
		it->second.inviteClient(*target_client);
	}

	// 초대 메시지 전송
	target_client->addToSendBuffer(":" + client.getNickname() + "!" + client.getUsername() + "@" + client.getHostname() + " INVITE " + nickname + " " + channel_name);
	client.addToSendBuffer(":" + client.getNickname() + " INVITE " + nickname + " " + channel_name);
}
