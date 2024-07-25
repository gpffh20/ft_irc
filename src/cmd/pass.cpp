#include "../../inc/server.hpp"

void Command::pass(int client_fd, std::vector<std::string> tokens) {
	
	(void) client_fd;
	server_.setPassWord(tokens[1]);
}