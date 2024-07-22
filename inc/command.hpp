#ifndef COMMAND_HPP
# define COMMAND_HPP

#include <unistd.h>
#include <vector>
#include <string>

#include "./server.hpp"

class Command {
  public:
	Command();
	~Command();
	
	void quit(int client_fd, std::vector<std::string> tokens);
	void pass(int client_fd, std::vector<std::string> tokens);
  private:
	Command &operator=(const Command&);
	Command(const Command&);
	
	Server		&server_;
};

#endif