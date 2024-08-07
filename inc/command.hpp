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
	
	void run(Client& client, std::vector<std::string> args);
	void quit(Client& client, std::vector<std::string> args);
	void pass(Client& client, std::vector<std::string> args);
	void nick(Client& client, std::vector<std::string> args);
	void part(Client& client, std::vector<std::string> args);
	void ping(Client& client, std::vector<std::string> args);
	void join(Client& client, std::vector<std::string> args);
	void privmsg(Client& client, std::vector<std::string> args);
	void user(Client& client, std::vector<std::string> args);
	void invite(Client& client, std::vector<std::string> args);
	
	bool isNicknameExist(std::string nickname, int fd);
	std::vector<std::string> parseForUSER(std::vector<std::string> args);
	std::vector<std::string> parseForQuit(std::vector<std::string> args);
	
	
	 std::string NEEDMOREPARAMS(std::string command);
	
  
  private:
	Command &operator=(const Command&);
	Command(const Command&);
	
	Server		&server_;
	
};

#endif
