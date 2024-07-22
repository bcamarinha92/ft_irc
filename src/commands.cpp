#include "../inc/ft_irc.hpp"
#include <iostream>

void    cmdNick(Server &irc, char *buffer, int sender)
{
    irc.setNickByFd(sender, getNickFromBuffer(buffer));
}

void    cmdJoin(Server &irc, char *buffer, int sender)
{
    std::string join = ":"+ irc.getNickByFd(sender) + " JOIN " + getChannelFromBuffer(buffer) + "\r\n";
    send(sender, join.c_str(), join.length(),MSG_DONTWAIT);
    Channel	channel(getChannelFromBuffer(buffer));
    if (irc.channels.find(getChannelFromBuffer(buffer)) == irc.channels.end())
    {
        irc.addChannel(channel);
        irc.activateChannelMode(getChannelFromBuffer(buffer), 'n', sender, true);
        irc.activateChannelMode(getChannelFromBuffer(buffer), 't', sender, true);
        irc.channels[getChannelFromBuffer(buffer)].addClient(irc.getClientByFd(sender));
        irc.channels[getChannelFromBuffer(buffer)].addOperator(irc.getClientByFd(sender));
    }
    else
    {
        irc.channels[getChannelFromBuffer(buffer)].addClient(irc.getClientByFd(sender));
        
    }
}

void    cmdWho(Server &irc, char *buffer, int sender)
{
    std::cout << getChannelFromBuffer(buffer) << std::endl;
    if (irc.channels.find(getChannelFromBuffer(buffer)) != irc.channels.end())
        who(sender, irc, getChannelFromBuffer(buffer), true);
}
