/* Server.cpp */

#include <arpa/inet.h>
#include <cstring>
#include <iostream>
#include <unistd.h>

int main() {
  // Create a socket
  int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
  if (serverSocket == -1) {
    std::cerr << "Error: Failed to create socket\n";
    return 1;
  }

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

  // Accept incoming connections
  sockaddr_in clientAddr;
  socklen_t clientAddrSize = sizeof(clientAddr);
  int clientSocket = accept(
      serverSocket, reinterpret_cast<sockaddr *>(&clientAddr), &clientAddrSize);
  if (clientSocket == -1) {
    std::cerr << "Error: Failed to accept connection\n";
    close(serverSocket);
    return 1;
  }

  std::cout << "Connection accepted from " << inet_ntoa(clientAddr.sin_addr)
            << ":" << ntohs(clientAddr.sin_port) << "\n";

  // Wait for client signal indicating readiness to receive
  char signal;
  if (recv(clientSocket, &signal, sizeof(signal), 0) == -1) {
    std::cerr << "Error: Failed to receive signal from client\n";
    close(clientSocket);
    close(serverSocket);
    return 1;
  }

  // Send data to client
  while (true) {
    std::string message;
    std::cout << "Enter a message to send to the client: ";
    std::getline(std::cin, message);
    if (message == "exit") {
      send(clientSocket, message.c_str(), message.size(), 0);
      break;
    }
    if (send(clientSocket, message.c_str(), message.size(), 0) == -1) {
      std::cerr << "Error: Failed to send data to client\n";
    }
  }

  // Close sockets
  close(clientSocket);
  close(serverSocket);

  return 0;
}
