#include "../../inc/command.hpp"

std::vector<std::string> Command::parseForPART(std::vector<std::string> args) {
	args[2] = args[2].substr(1);
	std::string msg = args[2];
	for (size_t i = 3; i < args.size(); ++i) {
		msg += " " + args[i];
	}
	if (args[2].find(',')) {
		std::vector<std::string> temp;
		temp.push_back(args[0]);
		temp.push_back(args[1]);
		std::string channel_list = args[2];
		std::string delimiter = ",";
		size_t pos = 0;
		std::string token;
		while ((
				pos = channel_list.find(delimiter)
		) != std::string::npos) {
			token = channel_list.substr(0, pos);
			temp.push_back(token);
			channel_list.erase(0, pos + delimiter.
					length()
			);
		}
		temp.push_back(channel_list);
		args = temp;
		args.push_back(msg);
	}
	return args;
}

void Command::part(Client &client, std::vector<std::string> args) {
	args = parseForPART(args);
	if (args.size() < 2) {
		client.addToSendBuffer(NEEDMOREPARAMS("PART"));
		return;
	}
	
	for (size_t i = 2; i < args.size() - 1; ++i) {
		std::string channel_name = "#" + args[i];
		std::map<std::string, Channel>::iterator it = server_.getChannels().find(channel_name);
		if (it == server_.getChannels().end()) {
			client.addToSendBuffer(
					ERR_NOSUCHCHANNEL + client.getNickname() + " " + channel_name + " :No such channel\r\n");
			return;
		}
		
		// channel안에 해당 client가 없으면 error
		std::vector < Client * > &clients = it->second.getClientList();
		bool client_found = false;
		std::vector<Client *>::iterator it_client;
		for (it_client = clients.begin(); it_client != clients.end(); ++it_client) {
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
		
		// 브로드캐스트 메시지 전송 (자신 제외)
		std::string partMessage =
				":" + client.getNickname() + "!" + client.getUsername() + "@" + client.getHostname() + " PART "
						+ channel_name + " :" + args[args.size() - 1] + "\r\n";
		for (it_client = clients.begin(); it_client != clients.end(); ++it_client) {
			if ((*it_client)->getNickname() != client.getNickname()) {
				(*it_client)->addToSendBuffer(partMessage);
			}
		}
		
		client.addToSendBuffer(partMessage);
		it->second.removeClient(client);
		client.removeChannel(&it->second);
		
		// 채널이 비었는지 검사 후 삭제
		if (it->second.getClientList().empty()) {
			server_.getChannels().erase(it);
		}
	}
}
