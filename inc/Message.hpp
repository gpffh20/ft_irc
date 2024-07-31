#ifndef MESSAGE_HPP
#define MESSAGE_HPP

#include <string>
#include <iostream>

class Message {
public:
	Message(int fd);
	~Message();
	
	void setBuffer(const std::string &buffer);
	std::string getBuffer() const;
//	std::string parseCmd();
	
private:
	int fd_;
	std::string buffer_;
};

#endif
