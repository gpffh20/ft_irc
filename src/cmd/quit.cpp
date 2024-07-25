#include "../../inc/server.hpp"

void Command::quit(int client_fd, std::vector<std::string> tokens) {
//	std::map<int, Client>& clients = server_.getClients();
	std::string quit_message = "Client has quit the chat";
	
	if (tokens.size() > 1) {
		quit_message = tokens[1];
		for (size_t i = 2; i < tokens.size(); ++i) {
			quit_message += " " + tokens[i];
		}
	}
	// 모든 사용자에게 quit 메세지 전송
	
	close(client_fd);
	server_.removeClient(client_fd);
}
