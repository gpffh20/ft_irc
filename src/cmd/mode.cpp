#include "../../inc/command.hpp"
#include "../../inc/server.hpp"
#include "../../inc/client.hpp"
#include "../../inc/channel.hpp"

// 채널 모드를 처리하는 함수
void Command::mode(Client &client, std::vector<std::string> args) {
	// MODE <channel> <option> <client>
	// 인자가 충분한지 확인
	if (args.size() < 2) {
		client.addToSendBuffer(
				std::string(ERR_NEEDMOREPARAMS) + " " + client.getNickname() + " MODE :Not enough parameters\n");
		return;
	}
	
	// target = channelName
	std::string target = args[1];
	bool isProcessed = false;
	
	// 사용자 모드 설정이 필요한 경우 (nickname 모드 처리)
	if (target[0] != '#') { // 채널 이름이 아니면 사용자 모드로 간주
		if (args.size() < 3) { // 모드 변경 파라미터 확인
			client.addToSendBuffer(
					std::string(ERR_NEEDMOREPARAMS) + " " + client.getNickname() + " MODE :Not enough parameters\n");
			return;
		}
		std::string mode = args[2];
		if ((mode == "+i" || mode == "-i") && args[1] == client.getNickname()) {
			// Invisible 모드 설정 또는 해제
			bool setInvisible = (mode == "+i");
			client.setInvisible(setInvisible);
			client.addToSendBuffer("MODE " + client.getNickname() + " " + mode + "\n");
			return;
		} else {
			client.addToSendBuffer(std::string(ERR_USERSDONTMATCH) + " " + client.getNickname()
										   + " :Cannot change mode for other users\n");
			return;
		}
	}
	
	// 채널 이름을 확인하고 해당 채널이 있는지 검사
	if (Server::getChannels().find(target) == Server::getChannels().end()) {
		client.addToSendBuffer(
				std::string(ERR_NOSUCHCHANNEL) + " " + client.getNickname() + " " + target + " :No such channel\n");
		return;
	}
	
	// 채널 객체 참조
	Channel &channel = Server::getChannels()[target];
	
	// 모드를 조회하는 경우 (인자가 2개일 때)
	if (args.size() == 2) {
		std::string modes = channel.getModeString(client);
		client.addToSendBuffer(
				std::string(RPL_CHANNELMODEIS) + " " + client.getNickname() + " " + target + " " + modes + "\n");
		return;
	}
	
	// 클라이언트가 채널의 운영자인지 확인
	if (!channel.isClientOp(client)) {
		client.addToSendBuffer(std::string(ERR_CHANOPRIVSNEEDED) + " " + client.getNickname() + " " + target
									   + " :You're not channel operator\n");
		return;
	}
	
	// 모드 문자열과 초기 설정
	std::string modes = args[2];
	size_t argIndex = 3;
	char currentSign = 0;
	// param = 모드 적용 대상 user
	std::string param;
	
	// 모드 문자열을 순회하면서 각 모드 처리
	for (size_t i = 0; i < modes.length(); ++i) {
		char mode = modes[i];
		// 현재 모드 변경 사인 (+, -) 설정
		if (mode == '+' || mode == '-') {
			currentSign = mode;
			continue;
		}
		
		// 모드가 추가 인자를 필요로 하는 경우 인자 가져오기
		if ((mode == 'k' && currentSign == '+') || mode == 'l' || mode == 'o') {
			if (argIndex < args.size()) {
				param = args[argIndex++];
			} else {
				client.addToSendBuffer(std::string(ERR_NEEDMOREPARAMS) + " " + client.getNickname()
											   + " MODE :Parameter needed for mode " + std::string(1, mode) + "\n");
				return;
			}
		}
		
		if (processMode(client, channel, mode, currentSign, param)) {
			isProcessed = true;
		}
	}
	
	// 모드 변경 메시지 구성 및 방송
	if (isProcessed) {
		std::string modeChangeMessage =
				":" + client.getNickname() + "!" + client.getUsername() + "@" + client.getHostname() + " MODE " + param
						+ " " + modes;
		// gykoh: 이 부분은 필요 없는 것 같아서 주석 처리했습니다.
//		for (size_t i = 3; i < args.size(); ++i) {
//			modeChangeMessage += " " + args[i];
//		}
		channel.sendToChannel(modeChangeMessage);
	}
}

// 각 모드 변경을 처리하는 함수
bool Command::processMode(Client &client, Channel &channel, char mode, char sign, const std::string &param) {
	bool isAdd = (sign == '+');
	switch (mode) {
		case 'i': channel.setInviteOnly(isAdd);
			return true;
		case 't': channel.setTopicProtected(isAdd);
			return true;
		case 'k':
			if (isAdd && !param.empty()) {
				channel.setChannelKey(param);
				return true;
			} else if (!isAdd) {
				channel.setChannelKey("");
				return true;
			}
			break;
		case 'l':
			if (isAdd) {
				int limit = atoi(param.c_str());
				if (limit > 0) {
					channel.setMaxClient(limit);
					return true;
				} else {
					client.addToSendBuffer(std::string(ERR_KEYSET) + " " + client.getNickname() + " :Invalid limit\n");
				}
			} else {
				channel.setMaxClient(0);
				return true;
			}
			break;
		case 'o':
			if (param.empty()) {
				client.addToSendBuffer(std::string(ERR_NOSUCHNICK) + " " + client.getNickname()
											   + " :Nickname required for operator mode\n");
			} else {
				Client *opClient = channel.getClientByNickname(param);
				if (opClient != NULL) {
					if (isAdd) {
						channel.addOp(*opClient);
					} else {
						channel.removeOp(*opClient);
					}
					return true;
				} else {
					client.addToSendBuffer(std::string(ERR_NOSUCHNICK) + " " + client.getNickname() + " " + param
												   + " :No such nick/channel\n");
				}
			}
			break;
	}
	return false;
}
