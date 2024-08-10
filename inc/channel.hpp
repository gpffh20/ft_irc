#ifndef __CHANNEL_HPP__
#define __CHANNEL_HPP__

#include <string>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <vector>
#include "client.hpp"

class Client;

class Channel {
private:
    std::vector<Client *> clientList_;
    std::vector<Client *> bannedUser_;
    std::vector<Client *> opUser_;
    std::string channelKey_;
    std::string channelName_;
    int         maxClient_;
    std::string topic_;
    std::string mode_;
    bool topicProtected_;
    bool inviteOnly_;

public:
    Channel();
    Channel(std::string channelName);
    ~Channel();
    std::string getChannelKey();
    void        setChannelKey(std::string key);
    std::string getChannelName() const;
    void        setChannelName(std::string name);
    int         getMaxClient();
    void        setMaxClient(int maxClient);
    std::string getTopic();
    void        setTopic(std::string topic);
    std::string getMode();
    void        setMode(std::string mode);
    bool        getInviteOnly();
    void        setInviteOnly(bool inviteOnly);
    bool        getTopicProtected();
    void        setTopicProtected(bool topicProtected);
	std::vector<Client *> &getClientList();

    void        addClient(Client &client);
	void        removeClient(Client &client);
    void        addOp(Client &client);
    void        removeOp(Client &client);
    void        banUser(Client &client);
    void        unbanUser(Client &client);
    void        kickUser(Client &client);
    void        kickUser(Client &client, std::string reason);
	void        removeUser(std::string nickname);
	void        sendToChannel(std::string& message);
	
	Client*		getClientByNickname(std::string nickname);

    bool        isClientInChannel(Client &client);
	bool        isClientOp(Client &client);
	bool        isClientBanned(Client &client);
	bool		isOp(std::string nickname);
	std::string getModeString(Client& client);
};

#endif
