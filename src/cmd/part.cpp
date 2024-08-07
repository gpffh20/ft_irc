#include "../../inc/command.hpp"

void Command::part(Client &client, std::vector<std::string> args) {
	if (args.size() < 2) {
		client.addToSendBuffer(NEEDMOREPARAMS("PART"));
		return;
	}
	std::string channel_name = args[1];
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
	client.addToSendBuffer(":" + client.getNickname() + "!" + client.getUsername() + "@" + client.getHostname() + " PART " + channel_name + "\r\n");
    it->second.removeClient(client);
    client.removeChannel(&it->second);
}