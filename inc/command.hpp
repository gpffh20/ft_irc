#ifndef COMMAND_HPP
# define COMMAND_HPP

#include <unistd.h>
#include <vector>
#include <string>
#include <iostream>
#include <map>

class Server;

class Command {
  public:
	Command(Server &server);
	~Command();
	
	void run(int client_fd, std::string command, std::vector<std::string> tokens);
	void quit(int client_fd, std::vector<std::string> tokens);
	void pass(int client_fd, std::vector<std::string> tokens);
	void nick(int client_fd, std::vector<std::string> tokens);
	void ping(int client_fd, std::vector<std::string> tokens);
	void join(int client_fd, std::vector<std::string> tokens);
  private:
	Command &operator=(const Command&);
	Command(const Command&);
	
	 Server		&server_;
};

#endif