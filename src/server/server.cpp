#include "../../inc/server.hpp"
#include "../../inc/command.hpp"

#include <arpa/inet.h>
#include <errno.h>
#include <string.h>

#include <stdexcept>

std::map<int, Client> Server::clients;
std::map<std::string, Channel> Server::channels;

Server::Server(const std::string &port_num, const std::string &password)
		: server_fd(-1), client_addr_size(sizeof(client_addr)), fd_count(0) {
	setPortNum(port_num);
	setPassWord(password);
	setServerSocket();
	setServerAddr();
	setServerBind();
	setServerListen();
	setServerFd();
	command_ = new Command(*this);
	
}

Server::~Server() {
	for (std::map<int, Client>::iterator it = clients.begin();
		 it != clients.end(); ++it) {
		close(it->first);
	}
	if (server_fd != -1)
		close(server_fd);
	close(server_fd);
}

void Server::run() {
	while (true) {
		// std::cout << "channel size: " << channels.size() << "\n";
		// event_count
		int poll_count = poll(fds, fd_count, -1); // 무한 대기 상태로 폴링
		if (poll_count == -1) {
			throw std::runtime_error("Poll failed: " + std::string(strerror(errno)));
		}
		// event 발생한 만큼 Loop
		for (int i = 0; i < fd_count; ++i) {
			if (fds[i].revents & POLLIN) {
				// std::cout << "POLLIN event on fd: " << fds[i].fd << std::endl;
				if (fds[i].fd == server_fd) {
					// 새로운 클라이언트 처리
					handleNewConnection();
				} else {
					// 기존 클라이언트의 요청 처리
					handleClientMessages(fds[i].fd);
				}
			} else if (fds[i].revents & (POLLERR | POLLHUP | POLLNVAL)) { // 에러 발생
				std::cout << "POLLERR | POLLHUP | POLLNVAL event on fd: " << fds[i].fd << std::endl;
				// 클라이언트 연결 종료
				removeClient(fds[i].fd);
				i--;
			}
		}
		sendToClients();
//		bool allClientsEnded = true;
//		for (std::map<int, Client>::iterator it = clients.begin(); it != clients.end(); ++it) {
//		    if (!it->second.getEnd()) {
//		        allClientsEnded = false;
//		        break;
//		    }
//		}
//
//		if (allClientsEnded) {
//    		break;
//		}
	}
}

std::map<int, Client> &Server::getClients() { return clients; }

std::map<std::string, Channel> &Server::getChannels() { return channels; }

void Server::setPortNum(const std::string &port_num) {
	std::istringstream iss(port_num);
	unsigned short num;
	if (!(iss >> num) || !iss.eof()) {
		throw std::invalid_argument("Invalid port number format.");
	}
	
	// 포트 번호가 숫자인지 확인
	// 0 ~ 1023: 잘 알려진 포트 번호
	if (num < 1024 || num > 65535) {
		throw std::invalid_argument("Invalid port number. Please use a port number "
									"between 1024 and 65535.");
	}
	this->portnum = num;
}

void Server::setPassWord(const std::string &password) {
	passWord = password;
}

std::string &Server::getPassWord() { return passWord; }

void Server::setServerSocket() {
	server_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (server_fd == -1) {
		throw std::runtime_error("Could not create socket");
	}
	int opt = 1;
	if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) == -1) {
		throw std::runtime_error("Setsockopt failed: " + std::string(strerror(errno)));
	}
}

void Server::setServerAddr() {
	std::memset(&server_addr, 0, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(portnum);
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
}

void Server::setServerBind() {
	if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) ==
			-1) {
		close(server_fd);
		throw std::runtime_error("Bind failed: " + std::string(strerror(errno)));
	}
}

void Server::setServerListen() {
	if (listen(server_fd, 10) == -1) {
		close(server_fd);
		throw std::runtime_error("Listen failed: " + std::string(strerror(errno)));
	}
	
	if (fcntl(server_fd, F_SETFL, O_NONBLOCK) == -1) {
		close(server_fd);
		throw std::runtime_error("Fcntl failed: " + std::string(strerror(errno)));
	}
}

void Server::setServerFd() {
	fds[0].fd = server_fd;
	fds[0].events = POLLIN;
	fd_count = 1;
}

void Server::addClient(int client_fd) {
	if (fd_count < MAX_CLIENTS) {
		clients.insert(std::make_pair(client_fd, Client(client_fd)));
		fds[fd_count].fd = client_fd;
		fds[fd_count].events = POLLIN;
		fd_count++;
	} else {
		close(client_fd); // 최대 클라이언트 수를 초과하면 연결 거부
	}
}

void Server::addNickname(const std::string &nickname) {
	nicknames.push_back(nickname);
}

void Server::removeClient(int client_fd) {
	std::map<int, Client>::iterator it = clients.find(client_fd);
	if (it != clients.end()) {
		it->second.sendMessage();  // 남아있는 메세지 전송
		close(client_fd);
		clients.erase(it);
	}
	for (int i = 0; i < fd_count; i++) {
		if (fds[i].fd == client_fd) {
			fds[i] = fds[fd_count - 1];
			fd_count--;
			break;
		}
	}
}
//	close(client_fd);
//	clients.erase(client_fd);
//	for (int i = 0; i < fd_count; i++) {
//		if (fds[i].fd == client_fd) {
//			fds[i] = fds[fd_count - 1];
//			fd_count--;
//			break;
//		}
//	}
//}

void Server::handleNewConnection() {
	int new_fd = accept(server_fd, (struct sockaddr *)&client_addr, &client_addr_size);
	if (new_fd == -1) {
		throw std::runtime_error("Accept failed: " + std::string(strerror(errno)));
	}
	fcntl(new_fd, F_SETFL, O_NONBLOCK);
	addClient(new_fd);
}

void Server::handleClientMessages(int client_fd) {
	Client &client = clients.find(client_fd)->second;
	
	char buffer[1024];
	int nbytes = read(client_fd, buffer, sizeof(buffer));
	if (nbytes <= 0) {
		if (nbytes == 0) {
			client.setEnd(true);
		} else {
			throw std::runtime_error("Read failed: " + std::string(strerror(errno))); //뭔가 문제있음 //접속 후 끊고 재접속하면 이 메시지 뜸
		}
	} else {
		if (std::string(buffer, nbytes).find("CAP LS") != std::string::npos) {
			removeClient(client_fd);
			return ;
		}
		try {
			client.setMessage(std::string(buffer, nbytes));
			handleCommands(client);
		} catch (const std::exception &e) {
			std::cerr << "Error handling command: " << e.what() << std::endl;
		}
	}
}

std::vector<std::string> Server::splitBySpace(const std::string &str) {
	std::vector<std::string> result;
	std::istringstream iss(str);
	std::string word;
	
	while (iss >> word) {
		result.push_back(word);
	}
	return result;
}

std::vector<std::string> Server::splitByCRLF(const std::string &str) {
	std::vector<std::string> result;
	std::istringstream iss(str);
	std::string line;
	
	// '\n'을 기준으로 라인을 읽은 후 각 라인의 끝에서 '\r' 제거
	while (std::getline(iss, line)) {
		if (!line.empty() && line.back() == '\r') {
			line.pop_back();  // 마지막 문자가 '\r'이면 제거
		}
		result.push_back(line);
	}
	return result;
}

void Server::handleCommands(Client &client) {
	std::vector<std::string> tokens = splitByCRLF(client.getMessage());
	if (tokens.empty()) {
		return;
	}
	for (size_t i = 0; i < tokens.size(); ++i) {
		std::cout << "<< " << tokens[i] << "\n";
		std::vector<std::string> args = splitBySpace(tokens[i]);
		if (args.empty()) {
			continue;
		}
		command_->run(client, args);
		if (client.getError()) {
			break;
		}
	}
	client.clearMessage();
}

void Server::sendToClient(int client_fd, const std::string &message) {
	send(client_fd, message.c_str(), message.length(), 0);
}

std::vector<std::string> &Server::getNicknames() { return nicknames; }

void Server::removeNickname(const std::string &nickname) {
	nicknames.erase(std::remove(nicknames.begin(), nicknames.end(), nickname), nicknames.end());
}

void Server::sendToClients() {
	std::map<int, Client> &clients = getClients();
	for (std::map<int, Client>::iterator it = clients.begin();
		 it != clients.end(); ++it) {
		it->second.sendMessage();
	}
}

void Server::addChannel(const Channel &channel) {
	channels.insert(std::make_pair(channel.getChannelName(), channel));
}

void Server::removeChannel(const std::string &channel_name) {
	channels.erase(channel_name);
}
