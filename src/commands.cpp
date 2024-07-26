#include "../inc/ft_irc.hpp"


void    cmdNick(Server &irc, Message *message, int sender)
{
    irc.setNickByFd(sender, getNickFromBuffer(message->get_buffer()));
    std::cout << "Registered user " << irc.getNickByFd(sender) << std::endl;
}

void    cmdJoin(Server &irc, Message *message, int sender)
{
    std::string chn = message->get_destination();
    std::string join = ":"+ irc.getNickByFd(sender) + " JOIN " + chn + "\r\n";
    send(sender, join.c_str(), join.length(),MSG_DONTWAIT);
    Channel	channel(chn);
    if (irc.channels.find(chn) == irc.channels.end())
    {
        irc.addChannel(channel);
        irc.activateChannelMode(chn, 'n', sender, true);
        irc.activateChannelMode(chn, 't', sender, true);
        irc.channels[chn].addClient(irc.getClientByFd(sender));
        irc.channels[chn].addOperator(irc.getClientByFd(sender));
    }
    else
        irc.channels[chn].addClient(irc.getClientByFd(sender));
}

void    cmdWho(Server &irc, std::string chn, int sender)
{

    std::cout << chn << std::endl;
    if (irc.channels.find(chn) != irc.channels.end())
        who(sender, irc, chn, true);
}

void    cmdPass(Server &irc, Message *message, int sender)
{
    if (message->get_parameters().size() == 1)
    {
        if (message->get_parameters()[0] == irc.getPassword())
        {
            std::string join = ":server 001 :Welcome to the Paulo Brificado's IRC " + irc.getNickByFd(sender) + "!\n"; // Está a sempre comer a primeira palavra idk why
            logConsole(join);
            send(sender, join.c_str(), join.length(), MSG_DONTWAIT);
        }
        else
        {
            std::string join = ":server 403 " + irc.getNickByFd(sender) + " :Invalid password\n";
            logConsole(join);
            send(sender, join.c_str(), join.length(), MSG_DONTWAIT);
            close(sender);
        }
    }
    else
    {
        std::string join = ":server 461 :Not enough parameters\n";
        logConsole(join);
        send(sender, join.c_str(), join.length(), MSG_DONTWAIT);
    }
}

void    cmdPrivMsg(Server &irc, Message *message, int sender)
{
    std::string join = ":" + irc.getNickByFd(sender) + " " + message->get_buffer() + "\n";
    for (size_t i = 0; i < irc.pollfds.size(); ++i)
    {
        if ((irc.pollfds[i].fd == sender) || (irc.pollfds[i].fd == irc.getServerSocket()))
            continue;
        send(irc.pollfds[i].fd, join.c_str(), join.size(), MSG_DONTWAIT);
    }
}