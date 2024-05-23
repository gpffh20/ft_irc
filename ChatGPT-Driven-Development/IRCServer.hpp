#ifndef IRC_SERVER_HPP
#define IRC_SERVER_HPP

#include <map>
#include <string>
#include <vector>
#include <stdexcept>

struct ClientInfo {
    std::string nickname;
    std::string username;
};

class IRCServer {
public:
    IRCServer(int port);
    ~IRCServer();
    void run();

private:
    int serverSocket;
    int kq;
    std::map<int, ClientInfo> clients;
    std::map<std::string, std::vector<int>> channels;

    void handleNewConnection();
    void handleClientMessage(int clientSocket);
    void closeClientConnection(int clientSocket);
    void broadcastMessage(const std::string& message, int senderSocket, const std::string& channel);
};

#endif // IRC_SERVER_HPP


//#ifndef __IRCSERVER_HPP__
//#define __IRCSERVER_HPP__
//
//#include <arpa/inet.h>
//#include <fcntl.h>
//#include <iostream>
//#include <map>
//#include <string>
//#include <sys/event.h>
//#include <sys/time.h>
//#include <unistd.h>
//#include <vector>

#define BOOTUP_MSG1 "\n███╗   ██╗ ██████╗      █████╗ ███╗   ██╗███████╗██╗    ██╗███████╗██████╗     ██╗██████╗  ██████╗\n"
#define BOOTUP_MSG2 "████╗  ██║██╔═══██╗    ██╔══██╗████╗  ██║██╔════╝██║    ██║██╔════╝██╔══██╗    ██║██╔══██╗██╔════╝\n"
#define BOOTUP_MSG3 "██╔██╗ ██║██║   ██║    ███████║██╔██╗ ██║███████╗██║ █╗ ██║█████╗  ██████╔╝    ██║██████╔╝██║     \n"
#define BOOTUP_MSG4 "██║╚██╗██║██║   ██║    ██╔══██║██║╚██╗██║╚════██║██║███╗██║██╔══╝  ██╔══██╗    ██║██╔══██╗██║     \n"
#define BOOTUP_MSG5 "██║ ╚████║╚██████╔╝    ██║  ██║██║ ╚████║███████║╚███╔███╔╝███████╗██║  ██║    ██║██║  ██║╚██████╗\n"
#define BOOTUP_MSG6 "╚═╝  ╚═══╝ ╚═════╝     ╚═╝  ╚═╝╚═╝  ╚═══╝╚══════╝ ╚══╝╚══╝ ╚══════╝╚═╝  ╚═╝    ╚═╝╚═╝  ╚═╝ ╚═════╝\n"
//
//
//class IRCServer {
//public:
//  IRCServer(int port);
//  ~IRCServer();
//  void run();
//
//private:
//  int serverSocket;
//  int kq;
//  struct ClientInfo {
//    std::string nickname;
//    std::string username;
//  };
//  std::map<int, ClientInfo> clients; // client socket -> client info
//  std::map<std::string, std::vector<int> > channels; // channel name -> client sockets
//
//  void handleNewConnection();
//  void handleClientMessage(int clientSocket);
//  void closeClientConnection(int clientSocket);
//};
//
//#endif // __IRCSERVER_HPP__
