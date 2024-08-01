#ifndef __CHANNEL_HPP__
#define __CHANNEL_HPP__

#include <string>
#include <iostream>
#include <algorithm>
#include <vector>
#include "client.hpp"

class Client;

class Channel {
private:
    std::vector<Client> clientList_;
    std::vector<Client> bannedUser_;
    std::vector<Client> opUser_;
    std::string channelPassword_;
    std::string channelName_;
    int         maxClient_;

public:
    Channel();
    Channel(std::string channelName);
    ~Channel();
    std::string getChannelPassword();
    void        setChannelPassword(std::string password);
    std::string getChannelName();
    void        setChannelName(std::string name);
    int         getMaxClient();
    void        setMaxClient(int maxClient);
};

#endif