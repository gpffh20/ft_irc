#include "../inc/client.hpp"

Client::Client(int socket_fd)
	: message_(socket_fd), fd(socket_fd), register_(false) {}

Client::~Client() {}

int Client::getFd() const { return fd; }

const std::string &Client::getNickname() const { return nickname_; }

bool Client::getIsRegistered() const { return register_; }

void Client::setNickname(const std::string &nickname) {
	nickname_ = nickname;
}

void Client::setMessage(const std::string &message) {
	message_.setSource(message);
}

void Client::setIsRegistered(bool is_registered) {
	register_ = is_registered;
}


