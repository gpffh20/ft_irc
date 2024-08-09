#include "../../inc/command.hpp"

Command::Command(Server &server) : server_(server) {}

Command::~Command() {}

void Command::run(Client &client, std::vector<std::string> args) {
	std::string command = args[0];
	if (!client.getIsRegistered()) {
		if (command == "PASS")
			pass(client, args);
		else if (command == "NICK")
			nick(client, args);
		else if (command == "USER")
			user(client, args);
		else {
			client.addToSendBuffer(ERR_NOTREGISTERED + client.getNickname() + " :You have not registered\r\n");
			client.setError(true);
		}
		if (client.getError())
			return;
		
		if (client.getIsRegistered()) {
			std::string msg;
			msg = ":localhost 001 " + client.getNickname() + " :Welcome to the FT_IRC Network, "
					+ client.getNickname() + "!" + client.getHostname()
					+ "@" + client.getServername() + "\r\n";
			client.addToSendBuffer(msg);
			msg = ":localhost 422 " + client.getNickname() + " :MOTD File is missing\r\n";
			client.addToSendBuffer(msg);
		}
	} else {
		if (command == "PASS") {
			pass(client, args);
		} else if (command == "NICK") {
			nick(client, args);
		} else if (command == "USER") {
			user(client, args);
		} else if (command == "JOIN") {
			join(client, args);
		} else if (command == "PART") {
			part(client, args);
		} else if (command == "PRIVMSG") {
			privmsg(client, args);
		} else if (command == "QUIT") {
			quit(client, args);
		} else if (command == "PING") {
			ping(client, args);
		} else if (command == "KICK") {
		} else if (command == "INVITE") {
			invite(client, args);
		} else if (command == "MODE") {
		} else {
			std::cerr << "Unknown command: " << command << std::endl;
		}
	}
}

std::string Command::NEEDMOREPARAMS(std::string command) {
	return ERR_NEEDMOREPARAMS + command + " :Not enough parameters\r\n";
}

bool Command::isNicknameExist(std::string nickname, int fd) {
	std::map<int, Client>::iterator it;
	for (it = server_.getClients().begin(); it != server_.getClients().end(); ++it) {
		if (it->second.getFd() == fd)
			continue;
		if (it->second.getNickname() == nickname) {
			return true;
		}
	}
	return false;
}
