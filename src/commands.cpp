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
        return ;
    send(sender, join.c_str(), join.length(), MSG_DONTWAIT);
}

void    cmdNick(Server &irc, Message *message, int sender)
{
    irc.setNickByFd(sender, getNickFromBuffer(message->get_buffer()));
    std::cout << "Registered user " << irc.getNickByFd(sender) << std::endl;
    std::string join = ":localhost 002 bde-sous :Your host is PauloBrificado, running version 0.01\n";
    logConsole(join);
    send(sender, join.c_str(), join.length(), MSG_DONTWAIT);
    time_t t = irc.getCreationDate();
    join = ":localhost 003 bde-sous :This server was created at " + std::string(std::ctime(&t)) + "\n";
    logConsole(join);
    send(sender, join.c_str(), join.length(), MSG_DONTWAIT);
    join = ":localhost 004 bde-sous localhost 0.01 ao itkol\n";
    logConsole(join);
    send(sender, join.c_str(), join.length(), MSG_DONTWAIT);
    join = ":localhost 005 bde-sous PREFIX=(ov)@+ CHANTYPES=#& CHANMODES=beI,k,l,imnprstz CASEMAPPING=ascii NETWORK=PauloBrificado\r\n";
    logConsole(join);
    send(sender, join.c_str(), join.length(), MSG_DONTWAIT);
    join = ":localhost 372 bde-sous :- Welcome to the IRC Server - Enjoy your stay!\r\n";
    logConsole(join);
    send(sender, join.c_str(), join.length(), MSG_DONTWAIT);
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
            std::string join = ":localhost 001 bde-sous :Welcome to the Paulo Brificado's IRC " + irc.getNickByFd(sender) + "!\n"; // Está a sempre comer a primeira palavra idk why
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