#include "../inc/server.hpp"

int main(int ac, char *av[]) {
  try {
    if (ac != 3) {
      std::cout << "Warning :: Argument is only two" << std::endl;
      return 1;
    }
    Server server(av[1], av[2]);
    server.run();
  } catch (std::exception &e) {
    std::cout << e.what() << std::endl;
  }
}
