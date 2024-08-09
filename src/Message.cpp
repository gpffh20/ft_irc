#include "../inc/Message.hpp"

Message::Message(int fd) : fd_(fd) {
	(void)fd_;
	buffer_ = "";
}

Message::~Message() {}

// client한테 받은 메세지 저장
void Message::setBuffer(const std::string &buffer) {
	buffer_ += buffer;
}

std::string Message::getBuffer() const {
	return buffer_;
}

void Message::clearBuffer() {
	buffer_ = "";
}
