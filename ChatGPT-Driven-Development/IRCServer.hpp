#ifndef IRC_SERVER_HPP
#define IRC_SERVER_HPP

#include <map>
#include <string>
#include <vector>
#include <stdexcept>

#define BOOTUP_MSG \
"\n███╗   ██╗ ██████╗      █████╗ ███╗   ██╗███████╗██╗    ██╗███████╗██████╗     ██╗██████╗  ██████╗\n" \
"████╗  ██║██╔═══██╗    ██╔══██╗████╗  ██║██╔════╝██║    ██║██╔════╝██╔══██╗    ██║██╔══██╗██╔════╝\n" \
"██╔██╗ ██║██║   ██║    ███████║██╔██╗ ██║███████╗██║ █╗ ██║█████╗  ██████╔╝    ██║██████╔╝██║     \n" \
"██║╚██╗██║██║   ██║    ██╔══██║██║╚██╗██║╚════██║██║███╗██║██╔══╝  ██╔══██╗    ██║██╔══██╗██║     \n" \
"██║ ╚████║╚██████╔╝    ██║  ██║██║ ╚████║███████║╚███╔███╔╝███████╗██║  ██║    ██║██║  ██║╚██████╗\n" \
"╚═╝  ╚═══╝ ╚═════╝     ╚═╝  ╚═╝╚═╝  ╚═══╝╚══════╝ ╚══╝╚══╝ ╚══════╝╚═╝  ╚═╝    ╚═╝╚═╝  ╚═╝ ╚═════╝\n"


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
    std::map<std::string, std::vector<int> > channels;

    void handleNewConnection();
    void handleClientMessage(int clientSocket);
    void closeClientConnection(int clientSocket);
    void broadcastMessage(const std::string& message, int senderSocket, const std::string& channel);
};

#endif // IRC_SERVER_HPP
