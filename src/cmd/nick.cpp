#include "../../inc/command.hpp"

void Command::nick(Client &client, std::vector<std::string> args) {
	// PASS 등록 안됨
	if (!client.getPass()) {
//		client.addToSendBuffer(std::string(ERR_NOTREGISTERED) + " * :You have not registered\r\n");
		client.setError(true);
		return;
	}
	// 인자 수 오류
	if (args.size() < 2) {
		client.addToSendBuffer(std::string(ERR_NONICKNAMEGIVEN) + " :No nickname given\r\n");
		return;
	}
	std::string nickname = args[1];
	// 닉네임 중복
	if (isNicknameExist(nickname, client.getFd())) {
		if (client.getNick()) {
			// 문구 수정하기
			client.addToSendBuffer(std::string(ERR_NICKNAMEINUSE) + " " + nickname + " :Nickname is already in use\r\n");
			return;
		} else {
			nickname = nickname + "_";
		}
	}
	if (client.getNick()) {
		server_.removeNickname(client.getNickname());
	}
	client.addToSendBuffer(":" + client.getNickname() + " NICK " + nickname + "\r\n");
	client.setNickname(nickname);
	server_.addNickname(nickname);
	std::vector<std::string> nicknames = server_.getNicknames();
	for (std::vector<std::string>::iterator it = nicknames.begin(); it != nicknames.end(); it++) {
		std::cout << "nickname: " << *it << std::endl;
	}
	client.setNick(true);
}
