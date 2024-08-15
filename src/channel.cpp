#include "../inc/channel.hpp"

Channel::Channel() : maxClient_(0), topicProtected_(false), inviteOnly_(false) {
	std::cout << "Anonymous channel created" << std::endl;
}

Channel::Channel(std::string channelName)
		: channelName_(channelName), maxClient_(0), topicProtected_(false), inviteOnly_(false) {
	std::cout << "Registered channel created, named " << channelName_ << std::endl;
}

Channel::~Channel() {}

std::string Channel::getChannelKey() {
	return channelKey_;
}

void Channel::setChannelKey(std::string key) {
	channelKey_ = key;
}

std::string Channel::getChannelName() const {
	return channelName_;
}

void Channel::setChannelName(std::string name) {
	channelName_ = name;
}

int Channel::getMaxClient() {
	return maxClient_;
}

void Channel::setMaxClient(int maxClient) {
	maxClient_ = maxClient;
}

std::string Channel::getTopic() {
	return topic_;
}

void Channel::setTopic(std::string topic) {
	topic_ = topic;
}

std::string Channel::getMode() {
	return mode_;
}

void Channel::setMode(std::string mode) {
	mode_ = mode;
}

bool Channel::getInviteOnly() {
	return inviteOnly_;
}

void Channel::setInviteOnly(bool inviteOnly) {
	inviteOnly_ = inviteOnly;
}

bool Channel::getTopicProtected() {
	return topicProtected_;
}

void Channel::setTopicProtected(bool topicProtected) {
	topicProtected_ = topicProtected;
}

void Channel::addClient(Client &client) {
	clientList_.push_back(&client);
}

void Channel::removeClient(Client &client) {
	std::vector<Client *>::iterator it = std::find(clientList_.begin(), clientList_.end(), &client);
	if (it != clientList_.end()) {
		*it = NULL;  // 해당 클라이언트 참조를 null로 설정
		clientList_.erase(it);  // 리스트에서 제거
	}
//	clientList_.erase(std::remove(clientList_.begin(), clientList_.end(), &client), clientList_.end());
	if (isClientOp(client)) {
		removeOp(client);
	}
}

void Channel::addOp(Client &client) {
	opUser_.push_back(&client);
}

void Channel::removeOp(Client &client) {
	opUser_.erase(std::remove(opUser_.begin(), opUser_.end(), &client), opUser_.end());
}

void Channel::inviteClient(Client &client) {
	if (std::find(invitedClients_.begin(), invitedClients_.end(), &client) == invitedClients_.end()) {
		invitedClients_.push_back(&client);
	}
}

bool Channel::isClientInvited(Client &client) {
	return std::find(invitedClients_.begin(), invitedClients_.end(), &client) != invitedClients_.end();
}

void Channel::banUser(Client &client) {
	bannedUser_.push_back(&client);
	// remove from client list
	clientList_.erase(std::remove(clientList_.begin(), clientList_.end(), &client), clientList_.end());
}

void Channel::unbanUser(Client &client) {
	bannedUser_.erase(std::remove(bannedUser_.begin(), bannedUser_.end(), &client), bannedUser_.end());
}

void Channel::kickUser(Client &client) {
	clientList_.erase(std::remove(clientList_.begin(), clientList_.end(), &client), clientList_.end());
}

void Channel::kickUser(Client &client, std::string reason) {
	std::cout << "Kicking user " << client.getNickname() << " for reason: " << reason << std::endl;
	clientList_.erase(std::remove(clientList_.begin(), clientList_.end(), &client), clientList_.end());
}

bool Channel::isClientInChannel(Client &client) {
	return std::find(clientList_.begin(), clientList_.end(), &client) != clientList_.end();
}

bool Channel::isClientOp(Client &client) {
	return std::find(opUser_.begin(), opUser_.end(), &client) != opUser_.end();
}

bool Channel::isClientBanned(Client &client) {
	return std::find(bannedUser_.begin(), bannedUser_.end(), &client) != bannedUser_.end();
}

std::vector<Client *> &Channel::getClientList() {
	return clientList_;
}

void Channel::removeUser(std::string nickname) {
	std::vector<Client *>::iterator iter = clientList_.begin();
	for (; iter != clientList_.end(); iter++) {
		if ((*iter)->getNickname() == nickname) {
			clientList_.erase(iter);
			break;
		}
	}
}

Client *Channel::getClientByNickname(std::string nickname) {
	std::vector<Client *>::iterator iter = clientList_.begin();
	for (; iter != clientList_.end(); iter++) {
		if ((*iter)->getNickname() == nickname) {
			return *iter;
		}
	}
	return NULL;
}

std::string Channel::getModeString(Client &client) {
	std::ostringstream modeString;
	bool isOp = isClientOp(client);
	bool spaceFlag = false;
	
	if (!channelKey_.empty()) {
		modeString << (spaceFlag ? " " : "") << "+k";
		if (isOp) {
			modeString << " " << channelKey_;
		}
		spaceFlag = true;
	}
	
	if (topicProtected_) {
		modeString << (spaceFlag ? " " : "") << "+t";
		spaceFlag = true;
	}
	
	if (inviteOnly_) {
		modeString << (spaceFlag ? " " : "") << "+i";
		spaceFlag = true;
	}
	
	if (maxClient_ > 0) {
		modeString << (spaceFlag ? " " : "") << "+l " << maxClient_;
		spaceFlag = true;
	}
	
	if (isOp) {
		modeString << (spaceFlag ? " " : "") << "+o";
		std::vector<Client *>::const_iterator it;
		for (it = opUser_.begin(); it != opUser_.end(); ++it) {
			modeString << " " << (*it)->getNickname();
		}
	}
	
	return modeString.str();
}

void Channel::sendToChannel(std::string &message) {
	for (std::vector<Client *>::iterator it = clientList_.begin(); it != clientList_.end(); ++it) {
		(*it)->addToSendBuffer(message);
	}
}

std::vector<int> Channel::getClientListId() {
	std::vector<int> clientListId;
	for (std::vector<Client *>::iterator it = clientList_.begin(); it != clientList_.end(); ++it) {
		clientListId.push_back((*it)->getFd());
	}
	return clientListId;
}
