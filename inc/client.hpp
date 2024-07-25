#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <string>

class Client {
 public:
  Client(int socket_fd, const std::string& nickname,
         const std::string& username)
      : fd(socket_fd), nick(nickname), user(username), is_registered(false) {}

  int getFd() const { return fd; }
  const std::string& getNick() const { return nick; }
  void setNick(const std::string& nickname) { nick = nickname; }
  const std::string& getUser() const { return user; }
  bool getIsRegistered() const { return is_registered; }
  void setRegistered(bool registered) { is_registered = registered; }

 private:
  int fd;
  std::string nick;
  std::string user;
  bool is_registered;
};

#endif
