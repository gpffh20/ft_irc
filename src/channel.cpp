#include "../inc/channel.hpp"

Channel::Channel() {
    std::cout << "Anonymous channel created" << std::endl;
}

Channel::Channel(std::string channelName) : channelName_(channelName) {
    std::cout << "Registered channel created, named " << channelName_ << std::endl;
}

Channel::~Channel() {
    std::cout << "Channel destroyed" << std::endl;
}

std::string Channel::getChannelPassword() {
    return channelPassword_;
}

void Channel::setChannelPassword(std::string password) {
    channelPassword_ = password;
}

std::string Channel::getChannelName() {
    return channelName_;
}

void Channel::setChannelName(std::string name) {
    channelName_ = name;
}

int Channel::getMaxClient() {
    return maxClient_;
}

void Channel::setMaxClient(int maxClient) {
    maxClient_ = maxClient;
}

std::string Channel::getTopic() {
    return topic_;
}

void Channel::setTopic(std::string topic) {
    topic_ = topic;
}

std::string Channel::getMode() {
    return mode_;
}

void Channel::setMode(std::string mode) {
    mode_ = mode;
}