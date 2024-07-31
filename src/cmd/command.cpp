#include "../../inc/command.hpp"

Command::Command(Server &server) : server_(server) {}

Command::~Command() {}

void Command::run(Client& client, std::vector<std::string> args) {
	// 명령어 확인용
	std::string command = args[0];
	if (command == "PASS") {
		pass(client, args);
	} else if (command == "NICK") {
		nick(client, args);
	} else if (command == "USER") {
		user(client, args);
	} else if (command == "JOIN") {
		std::cout << "JOIN command\n";
	} else if (command == "PRIVMSG") {
		std::cout << "PRIVMSG command\n";
		privmsg(client, args);
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

std::string Command::NEEDMOREPARAMS(std::string command) {
	return ERR_NEEDMOREPARAMS + command + " :Not enough parameters\r\n";
}

bool Command::isNicknameExist(std::string nickname) {
	std::map<int, Client>::iterator it;
	for (it = server_.getClients().begin(); it != server_.getClients().end(); ++it) {
		if (it->second.getNickname() == nickname) {
			return true;
		}
	}
	return false;
}