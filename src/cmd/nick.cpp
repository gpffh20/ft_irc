#include "../../inc/command.hpp"

void Command::nick(Client &client, std::vector<std::string> args) {
	// PASS 등록 안됨
	if (!client.getPass()) {
		client.addToSendBuffer(std::string(ERR_NOTREGISTERED) + " :You have not registered\r\n");
		return;
	}
	// 인자 수 오류
	if (args.size() < 2) {
		client.addToSendBuffer(std::string(ERR_NONICKNAMEGIVEN) + " :No nickname given\r\n");
		return;
	}
	// 닉네임 중복
	if (isNicknameExist(args[1], client.getFd())) {
		if (client.getNick()) {
			// 문구 수정하기
			client.addToSendBuffer(std::string(ERR_NICKNAMEINUSE) + " " + args[1] + " : is already in use\r\n");
		} else {
			std::string newNickname = args[1] + "_";
			client.addToSendBuffer(":" + client.getNickname() + " NICK " + newNickname + "\r\n");
			client.setNickname(newNickname);
			server_.addNickname(newNickname);
			client.setNick(true);
		}
		return ;
	}
	if (client.getNick()) {
		server_.removeNickname(client.getNickname());
	}
	client.addToSendBuffer(":" + client.getNickname() + " NICK " + args[1] + "\r\n");
	client.setNickname(args[1]);
	server_.addNickname(args[1]);
	std::vector<std::string> nicknames = server_.getNicknames();
	for (std::vector<std::string>::iterator it = nicknames.begin(); it != nicknames.end(); it++) {
		std::cout << "nickname: " << *it << std::endl;
	}
	client.setNick(true);
}
