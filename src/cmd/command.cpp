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
			client.addToSendBuffer(std::string(ERR_NOTREGISTERED) + " " + client.getNickname() + " " + command + " :You have not registered");
			client.setError(true);
		}
		if (client.getError())
			return;
		
		if (client.getIsRegistered()) {
			std::string msg;
			msg = ":localhost 001 " + client.getNickname() + " :Welcome to the FT_IRC Network, "
					+ client.getNickname() + "!" + client.getHostname()
					+ "@" + client.getServername();
			client.addToSendBuffer(msg);
			msg = ":localhost 422 " + client.getNickname() + " :MOTD File is missing";
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
			kick(client, args);
		} else if (command == "INVITE") {
			invite(client, args);
		} else if (command == "MODE") {
			mode(client, args);
		} else if (command == "TOPIC") {
			topic(client, args);
		} else {
			std::cerr << "Unknown command: " << command << std::endl;
		}
	}
}

std::string Command::NEEDMOREPARAMS(std::string command) {
	return ERR_NEEDMOREPARAMS + command + " :Not enough parameters";
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

Channel* Command::getChannelByName(std::string channelName) {
	std::map<std::string, Channel>::iterator it = server_.getChannels().find(channelName);
	if (it == server_.getChannels().end()) {
		return NULL;
	}
	return &it->second;
}