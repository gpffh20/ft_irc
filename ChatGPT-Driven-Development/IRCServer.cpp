#include "IRCServer.hpp"
#include <algorithm>
#include <cstring>
#include <sstream>

IRCServer::IRCServer(int port) {
  serverSocket = socket(AF_INET, SOCK_STREAM, 0);
  if (serverSocket == -1) {
    throw std::runtime_error("Failed to create socket");
  }

  // 소켓 옵션 설정 (소켓 재사용 가능)
  int opt = 1;
  if (setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) == -1) {
    close(serverSocket);
    throw std::runtime_error("Failed to set socket options");
  }

  fcntl(serverSocket, F_SETFL, O_NONBLOCK);

  sockaddr_in serverAddr;
  std::memset(&serverAddr, 0, sizeof(serverAddr));
  serverAddr.sin_family = AF_INET;
  serverAddr.sin_port = htons(port);
  serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

  if (bind(serverSocket, reinterpret_cast<sockaddr *>(&serverAddr), sizeof(serverAddr)) == -1) {
    close(serverSocket);
    throw std::runtime_error("Failed to bind socket");
  }

  if (listen(serverSocket, 5) == -1) {
    close(serverSocket);
    throw std::runtime_error("Failed to listen on socket");
  }

  kq = kqueue();
  if (kq == -1) {
    close(serverSocket);
    throw std::runtime_error("Failed to create kqueue");
  }

  struct kevent change;
  EV_SET(&change, serverSocket, EVFILT_READ, EV_ADD, 0, 0, NULL);
  if (kevent(kq, &change, 1, NULL, 0, NULL) == -1) {
    close(serverSocket);
    close(kq);
    throw std::runtime_error("Failed to register kevent");
  }
}

IRCServer::~IRCServer() {
  for (std::map<int, ClientInfo>::iterator it = clients.begin(); it != clients.end(); ++it) {
    close(it->first);
  }
  close(serverSocket);
  close(kq);
}

void IRCServer::run() {
  std::cout << "IRC Server running on port 6667..." << std::endl;

  while (true) {
    struct kevent event;
    int nev = kevent(kq, NULL, 0, &event, 1, NULL);
    if (nev > 0) {
      if (event.filter == EVFILT_READ) {
        if (event.ident == serverSocket) {
          handleNewConnection();
        } else {
          handleClientMessage(event.ident);
        }
      }
    }
  }
}

void IRCServer::handleNewConnection() {
  sockaddr_in clientAddr;
  socklen_t clientAddrSize = sizeof(clientAddr);
  int clientSocket = accept(serverSocket, reinterpret_cast<sockaddr *>(&clientAddr), &clientAddrSize);
  if (clientSocket == -1) {
    std::cerr << "Error: Failed to accept connection" << std::endl;
  } else {
    std::cout << "Connection accepted from " << inet_ntoa(clientAddr.sin_addr) << ":" << ntohs(clientAddr.sin_port) << std::endl;
    fcntl(clientSocket, F_SETFL, O_NONBLOCK);
    struct kevent change;
    EV_SET(&change, clientSocket, EVFILT_READ, EV_ADD, 0, 0, NULL);
    if (kevent(kq, &change, 1, NULL, 0, NULL) == -1) {
      std::cerr << "Error: Failed to register kevent for client" << std::endl;
      close(clientSocket);
    } else {
      clients[clientSocket] = ClientInfo(); // Initialize client info with empty nickname and username
    }
  }
}

void IRCServer::handleClientMessage(int clientSocket) {
  char buffer[1024] = {0};
  ssize_t bytesRead = recv(clientSocket, buffer, sizeof(buffer), 0);
  if (bytesRead <= 0) {
    closeClientConnection(clientSocket);
  } else {
    std::string message(buffer, bytesRead);
    ClientInfo &clientInfo = clients[clientSocket];

    if (clientInfo.nickname.empty() || clientInfo.username.empty()) {
      // Expecting NICK and USER commands
      if (message.find("NICK ") == 0) {
        clientInfo.nickname = message.substr(5);
        std::cout << "Client set nickname: " << clientInfo.nickname << std::endl;
      } else if (message.find("USER ") == 0) {
        size_t firstSpace = message.find(' ', 5);
        if (firstSpace != std::string::npos) {
          clientInfo.username = message.substr(5, firstSpace - 5);
          std::cout << "Client set username: " << clientInfo.username << std::endl;
        }
      } else {
        std::cerr << "Error: Expected NICK or USER command" << std::endl;
      }
    } else {
      // Handle channel messages
      if (message.find("JOIN ") == 0) {
        std::string channel = message.substr(5);
        channels[channel].push_back(clientSocket);
        std::cout << clientInfo.nickname << " joined channel: " << channel << std::endl;
      } else if (message.find("MSG ") == 0) {
        size_t spacePos = message.find(' ', 4);
        std::string channel = message.substr(4, spacePos - 4);
        std::string msg = message.substr(spacePos + 1);
        std::cout << clientInfo.nickname << " in " << channel << " says: " << msg << std::endl;

        std::string fullMsg = clientInfo.nickname + ": " + msg;

        for (std::vector<int>::iterator it = channels[channel].begin(); it != channels[channel].end(); ++it) {
          if (*it != clientSocket) {
            send(*it, fullMsg.c_str(), fullMsg.size(), 0);
          }
        }
      }
    }
  }
}

void IRCServer::closeClientConnection(int clientSocket) {
  std::cout << "Client disconnected: " << clients[clientSocket].nickname << std::endl;
  close(clientSocket);
  clients.erase(clientSocket);
  for (std::map<std::string, std::vector<int> >::iterator it = channels.begin(); it != channels.end(); ++it) {
    it->second.erase(std::remove(it->second.begin(), it->second.end(), clientSocket), it->second.end());
  }
}
