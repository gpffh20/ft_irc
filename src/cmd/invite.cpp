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
		client.addToSendBuffer(ERR_NOSUCHCHANNEL + client.getNickname() + " " + channel_name + " :No such channel\r\n");
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
				ERR_NOTONCHANNEL + client.getNickname() + " " + channel_name + " :You're not on that channel\r\n");
		return;
	}
	
	 // 클라이언트 목록을 닉네임 기준으로 찾음
    Client* target_client = NULL;
    std::map<int, Client>::iterator it2;
    for (it2 = server_.getClients().begin(); it2 != server_.getClients().end(); ++it2) {
        if (it2->second.getNickname() == nickname) {
            target_client = &it2->second;
            break;
        }
    }
    
    if (target_client == NULL) {
        client.addToSendBuffer(ERR_NOSUCHNICK + client.getNickname() + " " + nickname + " :No such nick/channel\r\n");
        return;
    }
    target_client->addToSendBuffer(":" + client.getNickname() + " INVITE " + nickname + " " + channel_name + "\r\n");
}