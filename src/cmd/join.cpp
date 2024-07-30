#include "../../inc/command.hpp"

void Command::join(int client_fd, std::vector<std::string> tokens) {
//    send(client_fd, "HELLO", 5, MSG_OOB);
    (void)tokens;
    write(client_fd, "HELLO", 5);
}