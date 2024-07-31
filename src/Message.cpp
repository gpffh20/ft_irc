#include "../inc/Message.hpp"

Message::Message(int fd) : fd_(fd) {
	std::cout << "Message fd: " << fd_ << std::endl;
}

Message::~Message() {}

void Message::setBuffer(const std::string &buffer) {
	buffer_ = buffer;
}

std::string Message::getBuffer() const {
	return buffer_;
}