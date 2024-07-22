#ifndef SERVER_HPP
#define SERVER_HPP

#include <fcntl.h>
#include <netinet/in.h>
#include <poll.h>
#include <sys/socket.h>
#include <unistd.h>

#include <iostream>
#include <map>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

#include "client.hpp"

const int MAX_CLIENTS = 256;

struct Channel {
  std::string name;
  std::vector<int> clients;
};

class Server {
public:
  Server(const std::string &port_num, const std::string &password);
  ~Server();

  void run();
  
  std::map<int, Client>& getClients();
  std::map<std::string, Channel>& getChannels();
  
  void removeClient(int client_fd);


private:
  void setPortNum(const std::string &port_num);
  void setPassWord(const std::string &password);
  void setServerSocket();
  void setServerAddr();
  void setServerBind();
  void setServerListen();
  void setServerFd();
  void addClient(int client_fd);
  void handleNewConnection();
  void handleClientMessages(int client_fd);
  void handleCommands(int client_fd, const std::string &message);
  std::vector<std::string> splitBySpace(const std::string &str);

  int server_fd;
  struct sockaddr_in server_addr;
  struct sockaddr_in client_addr;
  socklen_t client_addr_size;

  unsigned short portnum;
  std::string passWord;

  std::map<int, Client> clients;
  std::map<std::string, Channel> channels;
  struct pollfd fds[MAX_CLIENTS];
  int fd_count;
};

#endif
