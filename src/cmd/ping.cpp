#include "../../inc/command.hpp"

void Command::ping(int client_fd, std::vector<std::string> tokens) {
  if (tokens.size() > 1) {
    std::string serverIdentifier = tokens[1];
    std::string response = "PONG :" + serverIdentifier + "\r\n";
    send(client_fd, response.c_str(), response.size(), 0);
    std::cout << "Sent PONG to client " << client_fd << std::endl;
  } else {
    std::cerr << "Wrong PING command from client " << client_fd << std::endl;
  }
}
