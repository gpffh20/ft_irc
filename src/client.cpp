#include "../inc/client.hpp"

Client::Client(int socket_fd)
	: message_(socket_fd), fd(socket_fd), register_(false) {}

Client::~Client() {}

int Client::getFd() const { return fd; }

const std::string &Client::getNickname() const { return nickname_; }

bool Client::getIsRegistered() const { return register_; }

const std::string Client::getMessage() const {
	return message_.getBuffer();
}

void Client::setNickname(const std::string &nickname) {
	nickname_ = nickname;
}

void Client::setMessage(const std::string &message) {
	message_.setBuffer(message);
}

void Client::setUsername(const std::string &username) {
	username_ = username;
}

void Client::setRealname(const std::string &realname) {
	realname_ = realname;
}

void Client::setIsRegistered(bool is_registered) {
	register_ = is_registered;
}

void Client::addToSendBuffer(const std::string &message) {
	sendbuf_ += message;
	std::cout << "sendbuf: " << sendbuf_ << std::endl;
}
