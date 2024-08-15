#include "../../inc/command.hpp"

std::vector<std::string> Command::parseForPART(std::vector<std::string> args) {
	// Rejoin tokens if ':' is found
	if (args.size() > 2) {
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
	}
	std::vector<std::string> new_args;
	new_args.push_back(args[0]);
	if (args.size() < 3) {
		args.push_back("leaving");
	}
	std::string msg = args[2];
	std::string::size_type start = 0;
	std::string::size_type end = args[1].find(',');
	
	while (end != std::string::npos) {
		new_args.push_back(args[1].substr(start, end - start));
		start = end + 1;
		end = args[1].find(',', start);
	}
	new_args.push_back(args[1].substr(start));
	new_args.push_back(msg);
	return new_args;
}

void Command::part(Client &client, std::vector<std::string> args) {
	args = parseForPART(args);
	if (args.size() < 2) {
		client.addToSendBuffer(NEEDMOREPARAMS("PART"));
		return;
	}
	for (size_t i = 1; i < args.size() - 1; ++i) {
		std::string channel_name = args[i];
		std::map<std::string, Channel>::iterator it = server_.getChannels().find(channel_name);
		if (it == server_.getChannels().end()) {
			client.addToSendBuffer(std::string(ERR_NOSUCHCHANNEL) + " " + client.getNickname() + " " + channel_name
										   + " :No such channel");
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
					ERR_NOTONCHANNEL + client.getNickname() + " " + channel_name + " :You're not on that channel");
			return;
		}
		
		// 브로드캐스트 메시지 전송 (자신 제외)
		std::string partMessage =
				":" + client.getNickname() + "!" + client.getUsername() + "@" + client.getHostname() + " PART "
						+ channel_name + " :" + args[args.size() - 1];
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
