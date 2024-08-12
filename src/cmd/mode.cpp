#include "../../inc/command.hpp"
#include "../../inc/server.hpp"
#include "../../inc/client.hpp"
#include "../../inc/channel.hpp"

// 채널 모드를 처리하는 함수
void Command::mode(Client &client, std::vector<std::string> args) {
	if (args.size() < 2) {
		client.addToSendBuffer("461 " + client.getNickname() + " MODE :Not enough parameters\n");
		return;
	}

	std::string target = args[1];
	bool isProcessed = false;

	// 사용자 모드 설정
	if (target[0] != '#') {
		if (args.size() < 3) {
			client.addToSendBuffer("461 " + client.getNickname() + " MODE :Not enough parameters\n");
			return;
		}
		std::string mode = args[2];
		if ((mode == "+i" || mode == "-i") && args[1] == client.getNickname()) {
			client.setInvisible(mode == "+i");
			client.addToSendBuffer("MODE " + client.getNickname() + " " + mode + "\n");
			return;
		} else {
			client.addToSendBuffer("502 " + client.getNickname() + " :Cannot change mode for other users\n");
			return;
		}
	}

	// 채널 모드 설정
	if (Server::getChannels().find(target) == Server::getChannels().end()) {
		client.addToSendBuffer("403 " + client.getNickname() + " " + target + " :No such channel\n");
		return;
	}

	Channel &channel = Server::getChannels()[target];
	if (args.size() == 2) {
		std::string modes = channel.getModeString(client);
		client.addToSendBuffer("324 " + client.getNickname() + " " + target + " " + modes + "\n");
		return;
	}

	if (!channel.isClientOp(client)) {
		client.addToSendBuffer("482 " + client.getNickname() + " " + target + " :You're not channel operator\n");
		return;
	}

	std::string modes = args[2];
	size_t argIndex = 3;
	std::string modeChangeMessage = ":" + client.getNickname() + "!" + client.getUsername() + "@" + client.getHostname() + " MODE " + target;

	char currentSign = 0;
	std::string param;
	bool modeParamRequired = false;

	for (size_t i = 0; i < modes.size(); ++i) {
		char mode = modes[i];
		if (mode == '+' || mode == '-') {
			currentSign = mode;
			continue;
		}
		if (mode == 'l') {
			modeParamRequired = (currentSign == '+');
		} else if (mode == 'k' && currentSign == '+') {
			modeParamRequired = true;
		} else if (mode == 'o') {
			modeParamRequired = true;
		} else {
			modeParamRequired = false;
		}
		if (modeParamRequired) {
			if (argIndex < args.size()) {
				param = args[argIndex++];
			} else {
				client.addToSendBuffer(
					"461 " + client.getNickname() + " MODE :Parameter needed for mode " + std::string(1, mode) + "\n");
				return;
			}
		}

		if (processMode(client, channel, mode, currentSign, param)) {
			isProcessed = true;
			modeChangeMessage += " " + std::string(1, currentSign) + std::string(1, mode);
			if (modeParamRequired) {
				modeChangeMessage += " " + param;
			}
		}
	}

	if (isProcessed) {
		modeChangeMessage += "\r\n";
		channel.sendToChannel(modeChangeMessage);
	}
}

// 각 모드 변경을 처리하는 함수
bool Command::processMode(Client &client, Channel &channel, char mode, char sign, const std::string &param) {
	bool isAdd = (sign == '+');
	switch (mode) {
		case 'i':
			channel.setInviteOnly(isAdd);
			return true;
		case 't':
			channel.setTopicProtected(isAdd);
			return true;
		case 'k':
			if (isAdd) {
				if (!param.empty()) {
					channel.setChannelKey(param);
					return true;
				} else {
					client.addToSendBuffer("461 " + client.getNickname() + " :Key is required for +k mode\n");
					return false;
				}
			} else {
				channel.setChannelKey("");
				return true;
			}
		case 'l':
			if (isAdd) {
				if (!param.empty()) {
					int limit = std::stoi(param);
					if (limit > 0) {
						channel.setMaxClient(limit);
						return true;
					} else {
						client.addToSendBuffer("461 " + client.getNickname() + " :Invalid limit for +l mode\n");
						return false;
					}
				} else {
					client.addToSendBuffer("461 " + client.getNickname() + " :Parameter needed for mode +l\n");
					return false;
				}
			} else {
				channel.setMaxClient(0);
				return true;
			}
		case 'o':
			if (param.empty()) {
				client.addToSendBuffer("461 " + client.getNickname() + " :Nickname required for +o or -o mode\n");
				return false;
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
					client.addToSendBuffer("401 " + client.getNickname() + " " + param + " :No such nick/channel\n");
					return false;
				}
			}
		default:
			client.addToSendBuffer("472 " + client.getNickname() + " " + std::string(1, mode) + " :is unknown mode char to me\n");
			return false;
	}
}
