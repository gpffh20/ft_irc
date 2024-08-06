//#include "../../inc/command.hpp"
//
//void Command::part(Client& client, std::vector<std::string> args) {
//	if (args.size() < 2) {
//		client.addToSendBuffer(NEEDMOREPARAMS("PART"));
//		return;
//	}
//	std::string channel_name = args[1];
//	std::map<std::string, Channel>::iterator it = server_.getChannels().find(channel_name);
//	if (it == server_.getChannels().end()) {
//		client.addToSendBuffer(ERR_NOSUCHCHANNEL + client.getNickname() + " " + channel_name + " :No such channel\r\n");
//		return;
//	}
//	    // vector에서 클라이언트를 찾는 부분
//    std::vector<Client>& clientList = it->second.getClientList();
//    std::vector<Client>::iterator clientIt = clientList.end();
//    for (std::vector<Client>::iterator it = clientList.begin(); it != clientList.end(); ++it) {
//        if (it->getNickname() == client.getNickname()) {
//            clientIt = it;
//            break;
//        }
//    }
//
//    if (clientIt == clientList.end()) {
//        client.addToSendBuffer(ERR_NOTONCHANNEL + client.getNickname() + " " + channel_name + " :You're not on that channel\r\n");
//        return;
//    }
//
//	// 클라이언트를 채널에서 제거
//    it->second.removeClient(client.getNickname());
//    client.removeChannel(channel_name);
//
//	// PART 메시지 생성 및 채널에 있는 다른 클라이언트에게 전송
//	std::string partMessage =
//			":" + client.getNickname() + "!" + client.getUsername() + "@" + client.getServername() + " PART "
//					+ channel_name + "\r\n";
//	for (std::vector<Client>::iterator cli_it = clientList.begin(); cli_it != clientList.end(); ++cli_it) {
//		cli_it->addToSendBuffer(partMessage);
//	}
//}
////    if (it->second.getClientList().find(client.getNickname()) == it->second.getClientList().end()) {
////        client.addToSendBuffer(ERR_NOTONCHANNEL + client.getNickname() + " " + channel_name + " :You're not on that channel\r\n");
////        return;
////    }
////    it->second.removeUser(client.getNickname());
////    client.removeChannel(channel_name);
////}