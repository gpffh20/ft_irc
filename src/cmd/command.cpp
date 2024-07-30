#include "../../inc/command.hpp"

Command::Command(Server &server) : server_(server) {}

Command::~Command() {}

void Command::run(Client& client, std::string command, std::string token) {
	// 명령어 확인용
	std::cout << "receive from client : " << command << "\n";
	if (command == "PASS") {
		pass(client, token);
	} else if (command == "NICK") {
		nick(client, token);
	} else if (command == "USER") {
		user(client, token);
	} else if (command == "JOIN") {
		std::cout << "JOIN command\n";
	} else if (command == "PRIVMSG") {
		std::cout << "PRIVMSG command\n";
		privmsg(client, token);
	} else if (command == "QUIT") {
		std::cout << "QUIT command\n";
//		quit(client_fd, tokens);
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
