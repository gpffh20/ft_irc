#include "../../inc/server.hpp"

void Command::nick(Client& client, std::string token) {
	token.erase(0, 5);
	std::cout << "nick token: " << token << std::endl;
	client.setNickname(token);
	server_.addNickname(token);
}