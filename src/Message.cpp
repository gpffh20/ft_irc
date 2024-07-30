#include "../inc/Message.hpp"

Message::Message(int fd) : fd_(fd) {
	std::cout << "Message constructor" << std::endl;
	std::cout << "fd_ : " << fd_ << std::endl;
	std::cout << "source_ : " << source_ << std::endl;
}

Message::~Message() {}

void Message::setSource(const std::string &message) {
	// 수정 필요
	source_ = message;
}