#include "../inc/server.hpp"

Server::Server(const std::string port_num, const std::string password) {
	this->portnum = setPortNum(port_num);
	this->passWord = setPassWord(password);
	setServerSocket();
	setServerAddr();
	setServerBind();
	setServerListen();
	setServerFd();
}

Server::~Server()
{
	if (this->server_fd != -1)
		close(this->server_fd);
	if (this->client_fd != -1)
		close(this->client_fd);
}

void Server::run(){
	while (true) {
		this->client_fd = accept(this->server_fd, (struct sockaddr *)&this->client_addr, &this->client_addr_size);
		if (this->client_fd == -1)
			throw std::runtime_error("accept error");
		std::cout << "client_fd : " << this->client_fd << std::endl;
		
		// fcntl(this->client_fd, F_SETFL, O_NONBLOCK);
		// this->fds[this->fd_cnt].fd = this->client_fd;
		// this->fds[this->fd_cnt].events = POLLIN;
		// this->fd_cnt++;
		// addClient(this->client_fd);
		// std::cout << "fd[" << this->client_fd << "] is connected" << std::endl;
		
		// if (this->fd_cnt == 256)
		// 	throw std::runtime_error("fd_cnt is full");
		
//		poll 호출
//		poll 결과 확인
//		이벤트 발생시 처리
//		새 클라이언트 연결
//		기존 클라이언트 메시지 수신
//		명령어 실행 후 결과 전송
//		클라이언트 종료
	}
}