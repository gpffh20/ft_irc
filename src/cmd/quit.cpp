#include "../../inc/command.hpp"

std::vector<std::string> Command::parseForQUIT(std::vector<std::string> args) {
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

void Command::quit(Client &client, std::vector<std::string> args) {
	args = parseForQUIT(args);
	std::string msg = args[1].size() ? args[1] : "leaving";
	std::string quitMessage = ":" + client.getNickname() + "!" + client.getHostname() + "@" + client.getServername()
				+ " QUIT :Quit: " + msg;
	
	// 클라이언트가 참여한 채널에 있는 클라이언트에게 메세지 전달
	std::vector<Channel *> channels = client.getChannels();
	for (std::vector<Channel *>::iterator it = channels.begin(); it != channels.end(); ++it) {
		std::vector<Client *> &clientList = (*it)->getClientList();
		for (std::vector<Client *>::iterator it_client = clientList.begin(); it_client != clientList.end(); ++it_client) {
			if ((*it_client)->getFd() != client.getFd()) { // 본인은 제외하고 전달
				(*it_client)->addToSendBuffer(quitMessage);
			}
		}
		// 채널에서 나간 클라이언트 삭제
		(*it)->removeClient(client);
		// 아무도 없는 채널 삭제
		if ((*it)->getClientList().empty()) {
			server_.removeChannel((*it)->getChannelName());
		}
	}
	
	// quit 사용자에게 메세지 보내기
	client.addToSendBuffer("ERROR :Closing link: (" + client.getHostname() + "@" + client.getServername() + ") [Quit: " + msg + "]");
	
	// 클라이언트 연결 종료
	server_.removeClient(client.getFd());
}