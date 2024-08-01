#include "../inc/channel.hpp"

Channel::Channel() {
    std::cout << "Anonymous channel created" << std::endl;
}

Channel::Channel(std::string channelName) : channelName_(channelName) {
    std::cout << "Registered channel created, named " << channelName_ << std::endl;
}

std::string Channel::getChannelPassword() {
    return channelPassword_;
}

void Channel::setChannelPassword(std::string password) {
    channelPassword_ = password;
}

std::string Channel::getChannelName() {
    return channelName_
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