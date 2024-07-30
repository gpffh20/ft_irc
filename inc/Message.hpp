#ifndef MESSAGE_HPP
#define MESSAGE_HPP

#include <string>
#include <iostream>

class Message {
public:
	Message(int fd);
	~Message();
	
	void setSource(const std::string &message);
//	std::string parseCmd();
	
private:
	int fd_;
	std::string source_;
};

#endif
