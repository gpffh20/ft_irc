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
    std::string topic_;
    std::string mode_;

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
    std::string getTopic();
    void        setTopic(std::string topic);
    std::string getMode();
    void        setMode(std::string mode);

    void        addClient(Client client);
    void        removeClient(Client client);
    void        addOp(Client client);
    void        removeOp(Client client);
    void        banUser(Client client);
    void        unbanUser(Client client);
    void        kickUser(Client client);
    void        kickUser(Client client, std::string reason);

    bool        isClientInChannel(Client client);
    bool        isClientOp(Client client);
    bool        isClientBanned(Client client);
};

#endif