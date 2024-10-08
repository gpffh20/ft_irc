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
#include <cstring>

#include "client.hpp"
#include "channel.hpp"

class Command;

const int MAX_CLIENTS = 256;

class Server {
public:
  Server(const std::string &port_num, const std::string &password);
  ~Server();

  void run();
  
  static std::map<int, Client>& getClients();
  static std::map<std::string, Channel>& getChannels();

  std::vector<std::string>&       getNicknames();
  std::vector<std::string>        splitBySpace(const std::string &str);
  
  void          removeClient(int client_fd);
  void          setPassWord(const std::string &password);
  std::string&  getPassWord();
  void          addNickname(const std::string &nickname);
  void          sendToClient(int client_fd, const std::string& message);
  void          removeNickname(const std::string &nickname);
  void			addChannel(const Channel &channel);
  void      removeDisconnectedClientsFromChannels(int client_fd);
  

private:
  std::vector<std::string> splitByCRLF(const std::string &str);

  void  setPortNum(const std::string &port_num);
  void  setServerSocket();
  void  setServerAddr();
  void  setServerBind();
  void  setServerListen();
  void  setServerFd();
  void  addClient(int client_fd);
  void  handleNewConnection();
  void  handleClientMessages(int client_fd);
  void  handleCommands(Client &client);
  void  sendToClients();

  int                 server_fd;
  struct sockaddr_in  server_addr;
  struct sockaddr_in  client_addr;
  socklen_t           client_addr_size;

  unsigned short  portnum;
  std::string     passWord;

  static std::map<int, Client> clients;
  static std::map<std::string, Channel> channels;

  std::vector<std::string>        nicknames;
  struct pollfd                   fds[MAX_CLIENTS];
  int                             fd_count;
  
  Command	*command_;
};

#endif
