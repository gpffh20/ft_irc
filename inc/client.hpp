#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <string>
#include <unistd.h>
#include <sys/socket.h>


#include "Message.hpp"

class Client {
  public:
	Client(int socket_fd);
	~Client();
	
	bool operator==(const Client &other) const;
	
	// getter
	int getFd() const;
	const std::string &getNickname() const;
	bool getIsRegistered() const;
	const std::string getMessage() const;
	bool getPass() const;
	bool getNick() const;
	bool getUser() const;
	const std::string &getHostname() const;
	const std::string &getServername() const;
	
	// setter
	void setNickname(const std::string &nickname);
	void setMessage(const std::string &message);
	void setIsRegistered(bool is_registered);
	void setUsername(const std::string &username);
	void setHostname(const std::string &hostname);
	void setServername(const std::string &servername);
	void setRealname(const std::string &realname);
	void setPass(bool pass);
	void setNick(bool nick);
	void setUser(bool user);
	
	// send
	void addToSendBuffer(const std::string &message);
	void sendMessage();

	// message
	void clearMessage();
	
	bool error_;
  
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
	bool pass_;
	bool nick_;
	bool user_;
};

#endif
