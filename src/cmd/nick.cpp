#include "../../inc/command.hpp"

void Command::nick(int client_fd, std::vector<std::string> tokens) {
	(void) client_fd;
	std::cout << tokens[1] << std::endl;
//	server_.setNickName(tokens[1]);
}