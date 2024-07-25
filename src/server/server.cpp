#include "../../inc/server.hpp"
#include "../../inc/command.hpp"

#include <arpa/inet.h>
#include <errno.h>
#include <string.h>

#include <stdexcept>

Server::Server(const std::string &port_num, const std::string &password)
    : server_fd(-1), client_addr_size(sizeof(client_addr)), fd_count(0) {
  setPortNum(port_num);
  setPassWord(password);
  setServerSocket();
  setServerAddr();
  setServerBind();
  setServerListen();
  setServerFd();
}

Server::~Server() {
  for (std::map<int, Client>::iterator it = clients.begin();
       it != clients.end(); ++it) {
    close(it->first);
  }
  if (server_fd != -1)
    close(server_fd);
}

void Server::run() {
  while (true) {
    int poll_count = poll(fds, fd_count, -1); // 무한 대기 상태로 폴링
    if (poll_count == -1) {
      throw std::runtime_error("Poll failed: " + std::string(strerror(errno)));
    }

    for (int i = 0; i < fd_count; ++i) {
      if (fds[i].revents & POLLIN) {
        if (fds[i].fd == server_fd) {
          handleNewConnection();
        } else {
          handleClientMessages(fds[i].fd);
        }
      }
    }
  }
}

std::map<int, Client> &Server::getClients() { return clients; }

std::map<std::string, Channel> &Server::getChannels() { return channels; }

void Server::setPortNum(const std::string &port_num) {
  std::istringstream iss(port_num);
  unsigned short num;
  if (!(iss >> num) || !iss.eof()) {
    throw std::invalid_argument("Invalid port number format.");
  }

  // 포트 번호가 숫자인지 확인
  // 0 ~ 1023: 잘 알려진 포트 번호
  if (num < 1024 || num > 65535) {
    throw std::invalid_argument("Invalid port number. Please use a port number "
                                "between 1024 and 65535.");
  }
  this->portnum = num;
}

void Server::setPassWord(const std::string &password) { passWord = password; }

void Server::setServerSocket() {
  server_fd = socket(AF_INET, SOCK_STREAM, 0);
  if (server_fd == -1) {
    throw std::runtime_error("Could not create socket");
  }
  int opt = 1;
  setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
}

void Server::setServerAddr() {
  server_addr.sin_family = AF_INET;
  server_addr.sin_addr.s_addr = INADDR_ANY;
  server_addr.sin_port = htons(portnum);
  //      htons(static_cast<unsigned short>(std::atoi(portnum.c_str())));
}

void Server::setServerBind() {
  if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) ==
      -1) {
    throw std::runtime_error("Bind failed: " + std::string(strerror(errno)));
  }
}

void Server::setServerListen() {
  if (listen(server_fd, 10) == -1) {
    throw std::runtime_error("Listen failed: " + std::string(strerror(errno)));
  }
}

void Server::setServerFd() {
  fds[0].fd = server_fd;
  fds[0].events = POLLIN;
  fd_count = 1;
}

void Server::addClient(int client_fd) {
  if (fd_count < MAX_CLIENTS) {
    fds[fd_count].fd = client_fd;
    fds[fd_count].events = POLLIN;
    fd_count++;
  } else {
    close(client_fd); // 최대 클라이언트 수를 초과하면 연결 거부
  }
}

void Server::removeClient(int client_fd) {
  close(client_fd);
  for (int i = 0; i < fd_count; i++) {
    if (fds[i].fd == client_fd) {
      fds[i] = fds[fd_count - 1];
      fd_count--;
      break;
    }
  }
}

void Server::handleNewConnection() {
  int new_fd =
      accept(server_fd, (struct sockaddr *)&client_addr, &client_addr_size);
  if (new_fd == -1) {
    std::cerr << "Error accepting new connection: " << strerror(errno)
              << std::endl;
  } else {
    addClient(new_fd);
    std::cout << "New client connected: FD " << new_fd << std::endl;
  }
}

void Server::handleClientMessages(int client_fd) {
  char buffer[1024];
  int nbytes = read(client_fd, buffer, sizeof(buffer));
  if (nbytes <= 0) {
    if (nbytes == 0) {
      std::cout << "Socket " << client_fd << " closed remotely." << std::endl;
    } else {
      std::cerr << "Read error: " << strerror(errno) << std::endl;
    }
    removeClient(client_fd);
  } else {
    handleCommands(client_fd, std::string(buffer, nbytes));
  }
}

std::vector<std::string> Server::splitBySpace(const std::string &str) {
  std::vector<std::string> result;
  std::istringstream iss(str);
  std::string word;

  while (iss >> word) {
    result.push_back(word);
  }

  return result;
}

void Server::handleCommands(int client_fd, const std::string &message) {
  std::cout << "Received message from client " << client_fd << ": " << message << std::endl;
  std::vector<std::string> tokens = splitBySpace(message);
  if (tokens.empty()) {
      return;
  }
  // Rejoin tokens if ':' is found
  for (size_t i = 1; i < tokens.size(); ++i) {
    if (tokens[i][0] == ':') {
      std::string combined;
      for (size_t j = i; j < tokens.size(); ++j) {
        if (j > i) {
          combined += " ";
        }
        combined += tokens[j];
      }
      tokens.resize(i);
      tokens.push_back(combined.substr(1)); // Remove the leading ':'
      break;
    }
  }

  std::string command = tokens[0];
  std::cout << "Command: " << command << std::endl;
  command_->run(client_fd, command, tokens);
  
//  Command cmdrunner;
//  if (command == "PASS") {
//
//  } else if (command == "NICK") {
//
//  } else if (command == "USER") {
//
//  } else if (command == "JOIN") {
////    cmdrunner.join(client_fd, tokens);
//  } else if (command == "CAP") {
////    cmdrunner.join(client_fd, tokens);
//  } else if (command == "PRIVMSG") {
//
//  } else if (command == "QUIT") {
//    cmdrunner.quit(client_fd, tokens);
//  } else if (command == "PING") {
//    cmdrunner.ping(client_fd, tokens);
//  } else if (command == "KICK") {
//
//  } else if (command == "INVITE") {
//
//  } else if (command == "MODE") {
//
//  } else {
//    std::cerr << "Unknown command: " << command << std::endl;
//  }
}

