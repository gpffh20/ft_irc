#include "../../inc/command.hpp"

Command::Command(Server &server) : server_(server) {}

Command::~Command() {}

void Command::run(int client_fd, std::string command, std::vector<std::string> tokens) {
	// 명령어 확인용
	std::cout << "receive from client : " << command << "\n";
	if (command == "PASS") {
		pass(client_fd, tokens);
	} else if (command == "NICK") {
		nick(client_fd, tokens);
	} else if (command == "USER") {
		std::cout << "USER command\n";
	
	} else if (command == "JOIN") {
		std::cout << "JOIN command\n";
	
	} else if (command == "PRIVMSG") {
		std::cout << "PRIVMSG command\n";
	
	} else if (command == "QUIT") {
		std::cout << "QUIT command\n";
		quit(client_fd, tokens);
	} else if (command == "PING") {
		std::cout << "PING command\n";
		
	} else if (command == "KICK") {
		std::cout << "KICK command\n";
	
	} else if (command == "INVITE") {
		std::cout << "INVITE command\n";
	
	} else if (command == "MODE") {
		std::cout << "MODE command\n";
	
	} else {
		std::cerr << "Unknown command: " << command << std::endl;
	}
}