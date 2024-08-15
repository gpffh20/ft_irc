#include "../inc/client.hpp"

Client::Client(int socket_fd)
		: message_(socket_fd),
		  fd(socket_fd),
		  nickname_("nickname"),
		  end_(false),
		  register_(false),
		  pass_(false),
		  nick_(false),
		  user_(false),
		  error_(false),
		  invisible_(false) {}

Client::~Client() {}

int Client::getFd() const { return fd; }

const std::string &Client::getNickname() const { return nickname_; }

const std::string &Client::getUsername() const { return username_; }

bool Client::getIsRegistered() const { return register_; }

const std::string Client::getMessage() const {
	return message_.getBuffer();
}

void Client::setNickname(const std::string &nickname) {
	nickname_ = nickname;
}

void Client::setMessage(const std::string &message) {
	message_.setBuffer(message);
}

void Client::setUsername(const std::string &username) {
	username_ = username;
}

void Client::setHostname(const std::string &hostname) {
	hostname_ = hostname;
}

void Client::setServername(const std::string &servername) {
	servername_ = servername;
}

void Client::setRealname(const std::string &realname) {
	realname_ = realname;
}

void Client::setIsRegistered(bool is_registered) {
	register_ = is_registered;
}

// old addToSendBuffer
void Client::addToSendBuffer(const std::string &message) {
	std::cout << ">> " << message << std::endl;
	sendbuf_ += message + "\r\n";
	// std::cout << "sendbuf: " << sendbuf_ << std::endl;
}

void Client::setPass(bool pass) {
	pass_ = pass;
}

bool Client::getPass() const {
	return pass_;
}

void Client::setNick(bool nick) {
	nick_ = nick;
}

bool Client::getNick() const {
	return nick_;
}

void Client::setUser(bool user) {
	user_ = user;
}

bool Client::getUser() const {
	return user_;
}

// old sendMessage
void Client::sendMessage() {
	if (!sendbuf_.empty()) {
		// std::cout << "send to client : " << sendbuf_ << std::endl;
		const char *msg = sendbuf_.c_str();
		send(fd, msg, sendbuf_.size(), 0);
		sendbuf_.clear();
		// std::cout << "sendbuf clear" << std::endl;
		// std::cout << "sendbuf: " << sendbuf_ << std::endl;
	}
}

const std::string &Client::getHostname() const {
	return hostname_;
}

const std::string &Client::getServername() const {
	return servername_;
}

void Client::clearMessage() {
	message_.clearBuffer();
}

bool Client::operator==(const Client &other) const {
	return fd == other.fd;
}

void Client::addChannel(Channel *channel) {
	channels.push_back(channel);
}

std::vector<Channel *> Client::getChannels() {
	return channels;
}

void Client::removeChannel(Channel *channel) {
	channels.erase(std::remove(channels.begin(), channels.end(), channel), channels.end());
}

void Client::setEnd(bool end) {
	end_ = end;
}

bool Client::getEnd() const {
	return end_;
}

void Client::setError(bool error) {
	error_ = error;
}

bool Client::getError() const {
	return error_;
}

bool Client::getInvisible() const {
	return invisible_;
}

void Client::setInvisible(bool invisible) {
	invisible_ = invisible;
}

std::string Client::getForRead() {
	return for_read;
}

void Client::setForRead(const std::string &tmp) {
	for_read = tmp;
}
