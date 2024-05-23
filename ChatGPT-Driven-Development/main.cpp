#include "IRCServer.hpp"

int main(int ac , char **av) {
  if (ac != 2) {
    std::cerr << "Usage: " << av[0] << " <port>" << std::endl;
    return 1;
  }
  int port = std::stoi(av[1]);
  try {
    IRCServer server(port);
    server.run();
  } catch (const std::exception &e) {
    std::cerr << "Error: " << e.what() << std::endl;
    return 1;
  }
  return 0;
}
