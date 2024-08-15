#include "../../inc/client.hpp"
#include "../../inc/command.hpp"

void Command::privmsg(Client& client, std::vector<std::string> args) {
  if (args.size() < 3) {
    client.addToSendBuffer(":ircserv " + std::string(ERR_NEEDMOREPARAMS) + " " +
                           client.getNickname() +
                           " PRIVMSG :Not enough parameters\r\n");
    return;
  }

  std::string target = args[1];  // 메시지의 수신자 (닉네임 또는 채널)
  std::string message;

  // 메시지 파싱 및 유효성 검사
  if (args[2][0] == ':') {
    message = args[2].substr(1);  // ':' 제거
  } else {
    client.addToSendBuffer(":ircserv " + std::string(ERR_NOTEXTTOSEND) + " " +
                           client.getNickname() + " :No text to send\r\n");
    return;
  }

  for (size_t i = 3; i < args.size(); ++i) {
    message += " " + args[i];
  }

  std::stringstream ss(target);
  std::string receiver;
  while (std::getline(ss, receiver, ',')) {
    if (receiver[0] == '#' || receiver[0] == '&') {
      // 전역 채널 목록에 접근
      std::map<std::string, Channel>& channels = Server::getChannels();
      std::map<std::string, Channel>::iterator it = channels.find(receiver);
      if (it == channels.end()) {
        client.addToSendBuffer(":ircserv " + std::string(ERR_NOSUCHCHANNEL) +
                               " " + client.getNickname() + " " + receiver +
                               " :No such channel\r\n");
      } else {
        Channel* channel = &it->second;
        if (!channel->isClientInChannel(client)) {
          client.addToSendBuffer(":ircserv " + std::string(ERR_NOTONCHANNEL) +
                                 " " + client.getNickname() + " " +
                                 channel->getChannelName() +
                                 " :You're not on that channel\r\n");
        } else {
          std::vector<Client*>::iterator itClient;
          for (itClient = channel->getClientList().begin();
               itClient != channel->getClientList().end(); ++itClient) {
            if (*itClient != &client) {
              (*itClient)->addToSendBuffer(
                  ":" + client.getNickname() + "!" + client.getUsername() +
                  "@" + client.getServername() + " PRIVMSG " + receiver + " :" +
                  message + "\r\n");
            }
          }
        }
      }
    } else {
      // 전역 클라이언트 목록에 접근
      std::map<int, Client>& clients = Server::getClients();
      Client* toClient = NULL;
      std::map<int, Client>::iterator itClient;
      for (itClient = clients.begin(); itClient != clients.end(); ++itClient) {
        if (itClient->second.getNickname() == receiver) {
          toClient = &itClient->second;
          break;
        }
      }

      if (!toClient) {
        client.addToSendBuffer(":ircserv " + std::string(ERR_NOSUCHNICK) + " " +
                               client.getNickname() + " " + receiver +
                               " :No such nick/channel\r\n");
      } else {
        toClient->addToSendBuffer(":" + client.getNickname() + "!" +
                                  client.getUsername() + "@" +
                                  client.getServername() + " PRIVMSG " +
                                  receiver + " :" + message + "\r\n");
      }
    }
  }
}
