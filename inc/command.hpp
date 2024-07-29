#ifndef COMMAND_HPP
#define COMMAND_HPP

#include <sys/socket.h>
#include <unistd.h>

#include <iostream>
#include <map>
#include <string>
#include <vector>

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
  Command &operator=(const Command &);
  Command(const Command &);

  Server &server_;
};

#endif
