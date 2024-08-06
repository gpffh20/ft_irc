//#include "../../inc/command.hpp"
//
//void Command::part(Client& client, std::vector<std::string> args) {
//    if (args.size() < 2) {
//        client.addToSendBuffer(NEEDMOREPARAMS("PART"));
//        return;
//    }
//    std::string channel_name = args[1];
//    std::map<std::string, Channel>::iterator it = server_.getChannels().find(channel_name);
//    if (it == server_.getChannels().end()) {
//        client.addToSendBuffer(ERR_NOSUCHCHANNEL + client.getNickname() + " " + channel_name + " :No such channel\r\n");
//        return;
//    }
//    if (it->second.getUsers().find(client.getNickname()) == it->second.getUsers().end()) {
//        client.addToSendBuffer(ERR_NOTONCHANNEL + client.getNickname() + " " + channel_name + " :You're not on that channel\r\n");
//        return;
//    }
//    it->second.removeUser(client.getNickname());
//    client.removeChannel(channel_name);
//}