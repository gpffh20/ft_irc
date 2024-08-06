#include "../inc/channel.hpp"

Channel::Channel() {
	std::cout << "Anonymous channel created" << std::endl;
}

Channel::Channel(std::string channelName) : channelName_(channelName) {
	std::cout << "Registered channel created, named " << channelName_ << std::endl;
}

Channel::~Channel() {
	std::cout << "Channel destroyed" << std::endl;
}

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

void Channel::addClient(Client &client) {
	clientList_.push_back(client);
}

void Channel::removeClient(Client client) {
    clientList_.erase(std::remove(clientList_.begin(), clientList_.end(), client), clientList_.end());
}

void Channel::addOp(Client client) {
	opUser_.push_back(client);
}

void Channel::removeOp(Client client) {
	opUser_.erase(std::remove(opUser_.begin(), opUser_.end(), client), opUser_.end());
}

void Channel::banUser(Client client) {
	bannedUser_.push_back(client);
	// remove from client list
	clientList_.erase(std::remove(clientList_.begin(), clientList_.end(), client), clientList_.end());
}

void Channel::unbanUser(Client client) {
	bannedUser_.erase(std::remove(bannedUser_.begin(), bannedUser_.end(), client), bannedUser_.end());
}

void Channel::kickUser(Client client) {
	clientList_.erase(std::remove(clientList_.begin(), clientList_.end(), client), clientList_.end());
}

void Channel::kickUser(Client client, std::string reason) {
	std::cout << "Kicking user " << client.getNickname() << " for reason: " << reason << std::endl;
	clientList_.erase(std::remove(clientList_.begin(), clientList_.end(), client), clientList_.end());
}

bool Channel::isClientInChannel(Client client) {
	return std::find(clientList_.begin(), clientList_.end(), client) != clientList_.end();
}

bool Channel::isClientOp(Client client) {
	return std::find(opUser_.begin(), opUser_.end(), client) != opUser_.end();
}

bool Channel::isClientBanned(Client client) {
	return std::find(bannedUser_.begin(), bannedUser_.end(), client) != bannedUser_.end();
}

std::vector<Client> &Channel::getClientList() {
	return clientList_;
}