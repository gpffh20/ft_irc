#ifndef SERVER_HPP
#define SERVER_HPP

#include <iostream>

// 최대 client 수
// client 정보
// channel 정보
// server socket
//갖고 있어야 함

class Server {
  public:
	Server(const std::string port_num, const std::string password);
	~Server();
		
  private:
	Server();

};

#endif
