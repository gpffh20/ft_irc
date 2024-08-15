#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <string>
#include <unistd.h>
#include <vector>
#include <algorithm>
#include <sys/socket.h>


#include "Message.hpp"

class Channel;

class Client {
  public:
	Client(int socket_fd);
	~Client();
	
	bool operator==(const Client &other) const;
	
	// getter
	int getFd() const;
	const std::string &getNickname() const;
	const std::string &getUsername() const;
	bool getIsRegistered() const;
	const std::string getMessage() const;
	bool getPass() const;
	bool getNick() const;
	bool getUser() const;
	bool getEnd() const;
	bool getError() const;
	bool getInvisible() const;
	const std::string &getHostname() const;
	const std::string &getServername() const;
	std::vector<Channel*> getChannels();
	
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
	void setEnd(bool end);
	void setError(bool error);
	void setInvisible(bool invisible);
	
	// send
	void addToSendBuffer(const std::string &message);
	void sendMessage();

	// message
	void clearMessage();
	
	// channel
	void addChannel(Channel* channel);
	void removeChannel(Channel* channel);
	
  
  private:
	Message message_;
	std::vector<Channel *> channels; // client가 가입한 채널 목록
	
	int fd;
	std::string nickname_;
	std::string username_;
	std::string hostname_;
	std::string servername_;
	std::string realname_;
	std::string sendbuf_;

	bool end_;
	bool register_;
	bool pass_;
	bool nick_;
	bool user_;
	bool error_;
	bool invisible_;

};

#endif
