#ifndef COMMAND_HPP
# define COMMAND_HPP

#include <sys/socket.h>
#include <unistd.h>
#include <vector>
#include <string>
#include <iostream>
#include <map>

#include "errmsg.hpp"
#include "server.hpp"
#include "client.hpp"
#include "channel.hpp"

class Command {
  public:
	Command(Server &server);
	~Command();
	
	void run(Client &client, std::vector<std::string> args);
	void quit(Client &client, std::vector<std::string> args);
	void pass(Client &client, std::vector<std::string> args);
	void nick(Client &client, std::vector<std::string> args);
	void part(Client &client, std::vector<std::string> args);
	void ping(Client &client, std::vector<std::string> args);
	void join(Client &client, std::vector<std::string> args);
	void privmsg(Client &client, std::vector<std::string> args);
	void user(Client &client, std::vector<std::string> args);
	void invite(Client &client, std::vector<std::string> args);
	void kick(Client &client, std::vector<std::string> args);
	void mode(Client &client, std::vector<std::string> args);
	void topic(Client &client, std::vector<std::string> args);
	
	bool processMode(Client& client, Channel& channel, char mode, char sign, const std::string& param);
	bool isNicknameExist(std::string nickname, int fd);
	std::vector<std::string> parseForUSER(std::vector<std::string> args);
	std::vector<std::string> parseForQUIT(std::vector<std::string> args);
	std::vector<std::string> parseForKICK(std::vector<std::string> args);
	std::vector<std::string> parseForTOPIC(std::vector<std::string> args);
	std::vector<std::string> parseForPART(std::vector<std::string> args);

	std::string NEEDMOREPARAMS(std::string command);
	Channel *getChannelByName(std::string channelName);
  
  private:
	Command &operator=(const Command&);
	Command(const Command&);
	
	Server		&server_;
	
};

#endif
