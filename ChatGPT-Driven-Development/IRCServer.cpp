#include "IRCServer.hpp"
#include <algorithm>
#include <cstring>
#include <fcntl.h>
#include <iostream>
#include <sstream>
#include <unistd.h>
#include <sys/event.h>
#include <arpa/inet.h>

IRCServer::IRCServer(int port) {
    std::cout << BOOTUP_MSG1 << BOOTUP_MSG2 << BOOTUP_MSG3 << BOOTUP_MSG4 << BOOTUP_MSG5 << BOOTUP_MSG6 << std::endl;
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == -1) {
        throw std::runtime_error("Failed to create socket");
    }

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
    for (auto it = clients.begin(); it != clients.end(); ++it) {
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
            clients[clientSocket] = ClientInfo();
            std::string welcomeMessage = ":server 001 * :Welcome to the NO ANSWER IRC server\r\n";
            send(clientSocket, welcomeMessage.c_str(), welcomeMessage.size(), 0);
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
        std::cout << "Received message from client: " << message << std::endl; // 수신한 메시지 출력
        ClientInfo &clientInfo = clients[clientSocket];

        std::istringstream iss(message);
        std::string command;
        iss >> command;

        // Process NICK command
        if (command == "NICK") {
            std::string nickname;
            iss >> nickname;
            clientInfo.nickname = nickname;
            std::cout << "Client set nickname: " << clientInfo.nickname << std::endl;
            std::string welcomeMessage = ":server 001 " + clientInfo.nickname + " :Welcome to the IRC server\r\n";
            send(clientSocket, welcomeMessage.c_str(), welcomeMessage.size(), 0);
        } 
        // Process USER command
        else if (command == "USER") {
            std::string username, hostname, servername, realname;
            iss >> username >> hostname >> servername;
            std::getline(iss, realname);
            if (!username.empty() && !hostname.empty() && !servername.empty() && !realname.empty()) {
                clientInfo.username = username;
                std::cout << "Client set username: " << clientInfo.username << std::endl;
                std::string welcomeMessage = ":server 001 " + clientInfo.nickname + " :Welcome to the IRC server\r\n";
                send(clientSocket, welcomeMessage.c_str(), welcomeMessage.size(), 0);
            }
        } 
        // Process JOIN command
        else if (command == "JOIN") {
            std::string channel;
            iss >> channel;
            channels[channel].push_back(clientSocket);
            std::cout << clientInfo.nickname << " joined channel: " << channel << std::endl;
            std::string joinMessage = ":" + clientInfo.nickname + " JOIN " + channel + "\r\n";
            broadcastMessage(joinMessage, clientSocket, channel);
        } 
        // Process PRIVMSG command
        else if (command == "PRIVMSG") {
            std::string channel;
            iss >> channel;
            std::string msg;
            std::getline(iss, msg);
            if (!msg.empty() && msg[0] == ' ') {
                msg = msg.substr(1);
            }
            std::cout << clientInfo.nickname << " in " << channel << " says: " << msg << std::endl;

            std::string fullMsg = ":" + clientInfo.nickname + " PRIVMSG " + channel + " :" + msg + "\r\n";
            broadcastMessage(fullMsg, clientSocket, channel);
        }
        // handle QUIT command
        else if (command == "QUIT") {
            std::string quitMessage;
            std::getline(iss, quitMessage);
            closeClientConnection(clientSocket);
            std::cout << clientInfo.nickname << " has quit (" << quitMessage << ")" << std::endl;
        }
    }
}

void IRCServer::closeClientConnection(int clientSocket) {
    std::cout << "Client disconnected: " << clients[clientSocket].nickname << std::endl;
    close(clientSocket);
    clients.erase(clientSocket);
    for (auto it = channels.begin(); it != channels.end(); ++it) {
        it->second.erase(std::remove(it->second.begin(), it->second.end(), clientSocket), it->second.end());
    }
}

void IRCServer::broadcastMessage(const std::string& message, int senderSocket, const std::string& channel) {
    for (int clientSocket : channels[channel]) {
        if (clientSocket != senderSocket) {
            send(clientSocket, message.c_str(), message.size(), 0);
        }
    }
}
