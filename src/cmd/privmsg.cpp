#include "../../inc/server.hpp"

void Command::privmsg(Client& client, std::vector<std::string> args) {
  // 컴파일을 위해 client void 처리
  (void)client;
  if (args.size() < 3) {
    std::cerr << "Error: Not enough parameters for PRIVMSG command\n";
    return;
  }
  std::string target = args[1];
  std::string message = args[2];
  for (size_t i = 3; i < args.size(); ++i) {
    message += " " + args[i];
  }
  if (message[0] == ':') {
    message = message.substr(1);
  } else {
    std::cerr << "Error: Message format incorrect\n";
    return;
  }
  //   if (server_.isUser(target)) {
  //     server_.sendMessageToUser(client_fd, target, message);
  //   } else if (server_.isChannel(target)) {
  //     server_.sendMessageToChannel(client_fd, target, message);
  //   } else {
  //     std::cerr << "Error: No such user or channel\n";
  //   }
}

// server 쪽에 추가할 함수 예시
// bool Server::isUser(const std::string &nickname) {
//   return users_.find(nickname) != users_.end();
// }

// bool Server::isChannel(const std::string &channelName) {
//   return channels_.find(channelName) != channels_.end();
// }

// void Server::sendMessageToUser(int client_fd, const std::string &nickname, const std::string &message) {
//   auto it = users_.find(nickname);
//   if (it != users_.end()) {
//     int target_fd = it->second;
//     std::string full_message = ":" + std::to_string(client_fd) + " PRIVMSG " + nickname + " :" + message + "\r\n";
//     send(target_fd, full_message.c_str(), full_message.length(), 0);
//   } else {
//     std::cerr << "Error: No such user\n";
//   }
// }

// void Server::sendMessageToChannel(int client_fd, const std::string &channelName, const std::string &message) {
//   auto it = channels_.find(channelName);
//   if (it != channels_.end()) {
//     std::vector<int> &clients = it->second;
//     std::string full_message = ":" + std::to_string(client_fd) + " PRIVMSG " + channelName + " :" + message + "\r\n";
//     for (int target_fd : clients) {
//       if (target_fd != client_fd) {
//         send(target_fd, full_message.c_str(), full_message.length(), 0);
//       }
//     }
//   } else {
//     std::cerr << "Error: No such channel\n";
//   }
// }
