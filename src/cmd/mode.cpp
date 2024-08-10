#include "../../inc/command.hpp"
#include "../../inc/server.hpp"
#include "../../inc/client.hpp"
#include "../../inc/channel.hpp"

// 채널 모드를 처리하는 함수
void Command::mode(Client& client, std::vector<std::string> args) {
	// 인자가 충분한지 확인
	if (args.size() < 2) {
		client.addToSendBuffer(std::string(ERR_NEEDMOREPARAMS) + " " + client.getNickname() + " MODE :Not enough parameters\n");
		return;
	}

	// 채널 이름을 확인하고 해당 채널이 있는지 검사
	std::string channelName = args[1];
	if (Server::getChannels().find(channelName) == Server::getChannels().end()) {
		client.addToSendBuffer(std::string(ERR_NOSUCHCHANNEL) + " " + client.getNickname() + " " + channelName + " :No such channel\n");
		return;
	}

	// 채널 객체 참조
	Channel& channel = Server::getChannels()[channelName];

	// 모드를 조회하는 경우 (인자가 2개일 때)
	if (args.size() == 2) {
		std::string modes = channel.getModeString(client);
		client.addToSendBuffer(std::string(RPL_CHANNELMODEIS) + " " + client.getNickname() + " " + channelName + " " + modes + "\n");
		return;
	}

	// 클라이언트가 채널의 운영자인지 확인
	if (!channel.isClientOp(client)) {
		client.addToSendBuffer(std::string(ERR_CHANOPRIVSNEEDED) + " " + client.getNickname() + " " + channelName + " :You're not channel operator\n");
		return;
	}

	// 모드 문자열과 초기 설정
	std::string modes = args[2];
	size_t argIndex = 3;
	char currentSign = 0;
	std::string param;
	// bool modeChangeSuccess = false;

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
				client.addToSendBuffer(std::string(ERR_NEEDMOREPARAMS) + " " + client.getNickname() + " MODE :Parameter needed for mode " + std::string(1, mode) + "\n");
				return;
			}
		}

		// // 모드 변경 처리
		// modeChangeSuccess = processMode(client, channel, mode, currentSign, param);
		// if (!modeChangeSuccess) {
		// 	return; // 모드 변경 실패 시 처리 중단
		// }
	}

	// 모드 변경 성공 시, 채널의 모든 클라이언트에게 변경 사항 방송
	// if (modeChangeSuccess) {
	// 	// 모드 변경 메시지 구성 및 방송
	// 	std::string modeChangeMessage = "MODE " + channelName + " " + modes;
	// 	for (size_t i = 3; i < args.size(); ++i) {
	// 		modeChangeMessage += " " + args[i];
	// 	}
	// 	channel.broadcastToAllClients(modeChangeMessage);
	// }
}

// // 각 모드 변경을 처리하는 함수
// bool Command::processMode(Client& client, Channel& channel, char mode, char sign, const std::string& param) {
// 	bool isAdd = (sign == '+');
// 	switch (mode) {
// 		case 'i':
// 			return channel.setInviteOnly(isAdd);
// 		case 't':
// 			return channel.setTopicProtected(isAdd);
// 		case 'k':
// 			if (isAdd) {
// 				return channel.setKey(param);
// 			} else {
// 				return channel.removeKey();
// 			}
// 		case 'l':
// 			if (isAdd) {
// 				int limit = atoi(param.c_str());
// 				if (limit > 0) {
// 					return channel.setUserLimit(limit);
// 				} else {
// 					client.addToSendBuffer(std::string(ERR_KEYSET) + " " + client.getNickname() + " :Invalid limit\n");
// 					return false;
// 				}
// 			} else {
// 				return channel.removeUserLimit();
// 			}
// 		case 'o':
// 			if (param.empty()) {
// 				client.addToSendBuffer(std::string(ERR_NOSUCHNICK) + " " + client.getNickname() + " :Nickname required for operator mode\n");
// 				return false;
// 			}
// 			return channel.setOperator(param, isAdd);
// 		default:
// 			client.addToSendBuffer(std::string(ERR_UNKNOWNMODE) + " " + client.getNickname() + " " + mode + " :Unknown mode character\n");
// 			return false;
// 	}
// }
