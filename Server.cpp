#include <algorithm>
#include <arpa/inet.h>
#include <cstring>
#include <fcntl.h>
#include <iostream>
#include <sys/event.h>
#include <sys/time.h>
#include <unistd.h>
#include <vector>

int main() {
  // Create a socket
  int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
  if (serverSocket == -1) {
    std::cerr << "Error: Failed to create socket\n";
    return 1;
  }

  // Set socket to non-blocking mode
  int flags = fcntl(serverSocket, F_GETFL, 0);
  fcntl(serverSocket, F_SETFL, flags | O_NONBLOCK);

  // Bind the socket to an IP address and port
  sockaddr_in serverAddr;
  std::memset(&serverAddr, 0, sizeof(serverAddr));
  serverAddr.sin_family = AF_INET;
  serverAddr.sin_port = htons(12345); // Port number
  serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

  if (bind(serverSocket, reinterpret_cast<sockaddr *>(&serverAddr),
           sizeof(serverAddr)) == -1) {
    std::cerr << "Error: Failed to bind socket\n";
    close(serverSocket);
    return 1;
  }

  // Listen for incoming connections
  if (listen(serverSocket, 5) == -1) {
    std::cerr << "Error: Failed to listen on socket\n";
    close(serverSocket);
    return 1;
  }

  std::cout << "Server listening on port 12345...\n";

  // Create kqueue and register the server socket for read events (for new
  // connections)
  int kq = kqueue();
  if (kq == -1) {
    std::cerr << "Error: Failed to create kqueue\n";
    close(serverSocket);
    return 1;
  }

  struct kevent change;
  EV_SET(&change, serverSocket, EVFILT_READ, EV_ADD, 0, 0, NULL);
  if (kevent(kq, &change, 1, NULL, 0, NULL) == -1) {
    std::cerr << "Error: Failed to register kevent\n";
    close(serverSocket);
    close(kq);
    return 1;
  }

  std::vector<int> clientSockets;

  while (true) {
    struct kevent event;
    int nev = kevent(kq, NULL, 0, &event, 1, NULL);
    if (nev > 0) {
      if (event.filter == EVFILT_READ) {
        if (event.ident == serverSocket) {
          // Accept new client connection
          sockaddr_in clientAddr;
          socklen_t clientAddrSize = sizeof(clientAddr);
          int clientSocket =
              accept(serverSocket, reinterpret_cast<sockaddr *>(&clientAddr),
                     &clientAddrSize);
          if (clientSocket == -1) {
            std::cerr << "Error: Failed to accept connection\n";
          } else {
            std::cout << "Connection accepted from "
                      << inet_ntoa(clientAddr.sin_addr) << ":"
                      << ntohs(clientAddr.sin_port) << "\n";
            // Set client socket to non-blocking mode
            flags = fcntl(clientSocket, F_GETFL, 0);
            fcntl(clientSocket, F_SETFL, flags | O_NONBLOCK);
            // Register the client socket for read events
            EV_SET(&change, clientSocket, EVFILT_READ, EV_ADD, 0, 0, NULL);
            if (kevent(kq, &change, 1, NULL, 0, NULL) == -1) {
              std::cerr << "Error: Failed to register kevent for client\n";
              close(clientSocket);
            } else {
              clientSockets.push_back(clientSocket);
            }
          }
        } else {
          // Handle data from connected client
          int clientSocket = event.ident;
          char buffer[1024] = {0};
          ssize_t bytesRead = recv(clientSocket, buffer, sizeof(buffer), 0);
          if (bytesRead == -1) {
            std::cerr << "Error: Failed to receive data from client\n";
          } else if (bytesRead == 0) {
            std::cout << "Client has closed the connection\n";
            close(clientSocket);
            clientSockets.erase(std::remove(clientSockets.begin(),
                                            clientSockets.end(), clientSocket),
                                clientSockets.end());
          } else {
            std::string receivedData(buffer, bytesRead);
            std::cout << "Client says: " << receivedData << "\n";
            // Echo message to all other clients
            for (std::vector<int>::iterator it = clientSockets.begin();
                 it != clientSockets.end(); ++it) {
              if (*it != clientSocket) {
                if (send(*it, receivedData.c_str(), receivedData.size(), 0) ==
                    -1) {
                  std::cerr << "Error: Failed to send data to client\n";
                }
              }
            }
          }
        }
      }
    }
  }

  // Close client sockets
  for (std::vector<int>::iterator it = clientSockets.begin();
       it != clientSockets.end(); ++it) {
    close(*it);
  }
  // Close server socket and kqueue
  close(serverSocket);
  close(kq);

  return 0;
}
