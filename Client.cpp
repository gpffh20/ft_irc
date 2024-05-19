/* Client.cpp */

#include <arpa/inet.h>
#include <cstring>
#include <iostream>
#include <unistd.h>

int main() {
  // Create a socket
  int clientSocket = socket(AF_INET, SOCK_STREAM, 0);
  if (clientSocket == -1) {
    std::cerr << "Error: Failed to create socket\n";
    return 1;
  }

  // Connect to server
  sockaddr_in serverAddr;
  std::memset(&serverAddr, 0, sizeof(serverAddr));
  serverAddr.sin_family = AF_INET;
  serverAddr.sin_port = htons(12345);                  // Server port number
  serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1"); // Server IP address

  if (connect(clientSocket, reinterpret_cast<sockaddr *>(&serverAddr),
              sizeof(serverAddr)) == -1) {
    std::cerr << "Error: Failed to connect to server\n";
    close(clientSocket);
    return 1;
  }

  std::cout << "Connected to server\n";

  // Send signal to server indicating readiness to receive
  char signal = 'R';
  if (send(clientSocket, &signal, sizeof(signal), 0) == -1) {
    std::cerr << "Error: Failed to send signal to server\n";
    close(clientSocket);
    return 1;
  }

  // Receive data from server
  while (true) {
    char buffer[1024] = {0};
    if (recv(clientSocket, buffer, sizeof(buffer), 0) == -1) {
      std::cerr << "Error: Failed to receive data from server\n";
    } else {
      std::string receivedData(buffer);
      if (receivedData == "exit") {
        std::cout << "Server has ended the connection\n";
        break;
      }
      std::cout << "Server says: " << buffer << "\n";
    }
  }
  // Close socket
  close(clientSocket);

  return 0;
}
