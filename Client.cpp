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
  int clientSocket = socket(AF_INET, SOCK_STREAM, 0);
  if (clientSocket == -1) {
    std::cerr << "Error: Failed to create socket\n";
    return 1;
  }

  // Set socket to non-blocking mode
  fcntl(clientSocket, F_SETFL, O_NONBLOCK);

  // Connect to server
  sockaddr_in serverAddr;
  std::memset(&serverAddr, 0, sizeof(serverAddr));
  serverAddr.sin_family = AF_INET;
  serverAddr.sin_port = htons(12345);                  // Server port number
  serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1"); // Server IP address

  if (connect(clientSocket, reinterpret_cast<sockaddr *>(&serverAddr),
              sizeof(serverAddr)) == -1) {
    if (errno != EINPROGRESS) {
      std::cerr << "Error: Failed to connect to server\n";
      close(clientSocket);
      return 1;
    }
  }

  // Create kqueue and register the client socket for write events (for connect)
  int kq = kqueue();
  if (kq == -1) {
    std::cerr << "Error: Failed to create kqueue\n";
    close(clientSocket);
    return 1;
  }

  struct kevent change;
  EV_SET(&change, clientSocket, EVFILT_WRITE, EV_ADD, 0, 0, NULL);
  if (kevent(kq, &change, 1, NULL, 0, NULL) == -1) {
    std::cerr << "Error: Failed to register kevent\n";
    close(clientSocket);
    close(kq);
    return 1;
  }

  bool connected = false;
  struct kevent event;
  while (!connected) {
    int nev = kevent(kq, NULL, 0, &event, 1, NULL);
    if (nev > 0) {
      if (event.filter == EVFILT_WRITE) {
        int result;
        socklen_t result_len = sizeof(result);
        getsockopt(clientSocket, SOL_SOCKET, SO_ERROR, &result, &result_len);
        if (result == 0) {
          connected = true;
          std::cout << "Connected to server\n";
        } else {
          std::cerr << "Error: Failed to connect to server\n";
          close(clientSocket);
          close(kq);
          return 1;
        }
      }
    }
  }

  // Register the client socket for read events (for receiving data)
  EV_SET(&change, clientSocket, EVFILT_READ, EV_ADD, 0, 0, NULL);
  if (kevent(kq, &change, 1, NULL, 0, NULL) == -1) {
    std::cerr << "Error: Failed to register kevent for read\n";
    close(clientSocket);
    close(kq);
    return 1;
  }

  // Register stdin for read events
  EV_SET(&change, STDIN_FILENO, EVFILT_READ, EV_ADD, 0, 0, NULL);
  if (kevent(kq, &change, 1, NULL, 0, NULL) == -1) {
    std::cerr << "Error: Failed to register kevent for stdin\n";
    close(clientSocket);
    close(kq);
    return 1;
  }

  // Receive data from server
  while (true) {
    int nev = kevent(kq, NULL, 0, &event, 1, NULL);
    if (nev > 0) {
      if (event.filter == EVFILT_READ) {
        if (event.ident == clientSocket) {
          char buffer[1024] = {0};
          ssize_t bytesRead = recv(clientSocket, buffer, sizeof(buffer), 0);
          if (bytesRead == -1) {
            std::cerr << "Error: Failed to receive data from server\n";
          } else if (bytesRead == 0) {
            std::cout << "Server has closed the connection\n";
            break;
          } else {
            std::string receivedData(buffer, bytesRead);
            std::cout << "Server says: " << receivedData << "\n";
          }
        } else if (event.ident == STDIN_FILENO) {
          std::string message;
          std::getline(std::cin, message);
          if (send(clientSocket, message.c_str(), message.size(), 0) == -1) {
            std::cerr << "Error: Failed to send message to server\n";
          }
          if (message == "exit") {
            break;
          }
        }
      }
    }
  }

  // Close socket and kqueue
  close(clientSocket);
  close(kq);

  return 0;
}
