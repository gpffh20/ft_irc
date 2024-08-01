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
//	void quit(int client_fd, std::vector<std::string> tokens);
	void pass(Client& client, std::vector<std::string> args);
	void nick(Client& client, std::vector<std::string> args);
	void ping(int client_fd, std::vector<std::string> args);
	void join(Client& client, std::vector<std::string> args);
	void privmsg(Client& client, std::vector<std::string> args);
	void user(Client& client, std::vector<std::string> args);
	
	bool isNicknameExist(std::string nickname);
	
	 std::string NEEDMOREPARAMS(std::string command);
	
  
  private:
	Command &operator=(const Command&);
	Command(const Command&);
	
	Server		&server_;
	
};

#endif