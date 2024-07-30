#ifndef COMMAND_HPP
# define COMMAND_HPP

#include <sys/socket.h>
#include <unistd.h>
#include <vector>
#include <string>
#include <iostream>
#include <map>

class Server;
class Client;

class Command {
  public:
	Command(Server &server);
	~Command();
	
	void run(Client& client, std::string command, std::string token);
//	void quit(int client_fd, std::vector<std::string> tokens);
	void pass(Client& client, std::string token);
	void nick(Client& client, std::string token);
	void ping(int client_fd, std::vector<std::string> tokens);
	void join(int client_fd, std::vector<std::string> tokens);
	void privmsg(int client_fd, std::vector<std::string> tokens);
  private:
	Command &operator=(const Command&);
	Command(const Command&);
	
	 Server		&server_;
};

#endif