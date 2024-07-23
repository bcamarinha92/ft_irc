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
            std::string join = "Welcome to the Paulo Brificado's IRC !\n";
            logConsole(join);
            //send(sender, join.c_str(), join.length(), MSG_DONTWAIT);
        }
        else
        {
            std::string join = "Invalid password !\n";
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

void	cmdCap(Message *message) {
	logConsole("CAP * LS :");
	send(message->get_sender(), "CAP * LS :", 10, MSG_DONTWAIT);
}

void	cmdUser(Message *message, Client *user) { //Esta a dar segfault :c
	std::vector<std::string> parameters = message->get_parameters();
	
	user->setUsername(parameters[0]);
	user->setHostname(parameters[1]);
	user->setServername(parameters[2]);
	user->setRealname(parameters[3]);

	std::string join = ":server 001 " + message->get_nickname() + " :Welcome to Paulo Brificado's IRC network " + message->get_nickname();
	send(message->get_sender(), &join, join.size(), MSG_DONTWAIT);
}