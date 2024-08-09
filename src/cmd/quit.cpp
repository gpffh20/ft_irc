#include "../../inc/command.hpp"

std::vector<std::string> Command::parseForQUIT(std::vector<std::string> args) {
	for (size_t i = 2; i < args.size(); ++i) {
		args[1] += " " + args[i];
	}
	args.resize(2);
	return args;
}

void Command::quit(Client &client, std::vector<std::string> args) {
	std::string quitMessage;
	if (args.size() < 2) {
		quitMessage = ":" + client.getNickname() + "!" + client.getHostname() + "@" + client.getServername()
				+ " QUIT :Client Quit\r\n";
	} else {
		args = parseForQUIT(args);
		quitMessage = ":" + client.getNickname() + "!" + client.getHostname() + "@" + client.getServername() + " QUIT "
				+ args[1] + "\r\n";
	}
	client.addToSendBuffer(quitMessage);
	client.sendMessage();  // 남아있는 메시지 즉시 전송
	server_.removeClient(client.getFd());  // 클라이언트 연결 종료
}