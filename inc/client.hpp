#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <string>
#include <unistd.h>

#include "Message.hpp"

class Client {
  public:
	Client(int socket_fd);
	~Client();
	
	// getter
	int getFd() const;
	const std::string &getNickname() const;
	bool getIsRegistered() const;
	const std::string getMessage() const;
	
	// setter
	void setNickname(const std::string &nickname);
	void setMessage(const std::string &message);
	void setIsRegistered(bool is_registered);
	void setUsername(const std::string &username);
	void setRealname(const std::string &realname);
	
	// send
	void addToSendBuffer(const std::string &message);
	
	std::string send_buffer_;
  
  private:
	Message message_;
	
	int fd;
	std::string nickname_;
	std::string username_;
	std::string hostname_;
	std::string servername_;
	std::string realname_;
	std::string sendbuf_;
	
	bool register_;
};

#endif
