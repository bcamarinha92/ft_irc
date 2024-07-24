#include "../inc/ft_irc.hpp"

void    cmdCap(Server &irc, Message *message, int sender)
{
    std::string join;
    (void)irc;
    if (message->get_buffer().find("CAP REQ")!= std::string::npos)
        join = ":bde-sous CAP ACK :multi-prefix\r\n";    
    else if (message->get_buffer().find("CAP LS")!= std::string::npos)
        join = ":bde-sous CAP * LS :multi-prefix\r\n";
    else
    {
        sendSequenceRPL(irc, message, sender);
        sendMOTD(irc, message, sender);
        return ;
    }
    send(sender, join.c_str(), join.length(), MSG_DONTWAIT);
}

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
        if (message->get_parameters()[0] != irc.getPassword())
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

void    cmdPing(Message *message, int sender)
{
    std::string join = ":localhost ";
    join += message->get_buffer();
    size_t pos = join.find("PING");
    join.replace(pos, 4, "PONG");
    send(sender, join.c_str(), join.length(), MSG_DONTWAIT);
}
