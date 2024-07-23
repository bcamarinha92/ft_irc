#include "../inc/ft_irc.hpp"


void    cmdNick(Server &irc, Message *message, int sender)
{
    irc.setNickByFd(sender, getNickFromBuffer(message->get_buffer()));
}

void    cmdJoin(Server &irc, Message *message, int sender)
{
    std::string chn = message->get_destination();
    std::string join = ":" + irc.getNickByFd(sender) + " JOIN " + chn + "\r\n";
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

void    cmdWho(Server &irc, Message *message, int sender)
{
	if (irc.channels.find(message->get_destination()) != irc.channels.end())
    {
		std::string						msg;
		std::string						clients = "";
		std::map<int, Client>			clientsMap = irc.channels[message->get_destination()].members;
		for (std::map<int, Client>::iterator it = clientsMap.begin(); it != clientsMap.end(); ++it)
    	{
			if (it != clientsMap.begin())
				clients += " ";
        	if (irc.channels[message->get_destination()].checkOperatorRole((it->first)))
        		clients += "@" + (it->second).getNickname();
        	else
            	clients += (it->second).getNickname();
    	}
		logConsole("clientes: " + clients);
		msg = ":" + irc.getHostname() + " 353 " + irc.getNickByFd(sender) + " = " + message->get_destination() + " :" + clients + "\r\n";
		send(sender, msg.c_str(), msg.size(), MSG_DONTWAIT);
	}
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

void	cmdMode(Server &irc, Message *message, int sender)
{
	std::string	mode;
	if (message->get_parameters().size() > 1)
		mode = message->get_parameters()[1];
	std::string chn = message->get_destination();
	if (mode.size() > 1 && (!mode.compare(0, 1, "+", 0, 1) || !mode.compare(0, 1, "-", 0, 1)))
	{
		char	m = mode[1];
		if (!mode.compare(0, 1, "+", 0, 1))
			irc.activateChannelMode(chn, m, sender, false);
		else
			irc.deactivateChannelMode(chn, m, sender);
	}
	else if (message->get_parameters().size() == 1)
	{
		std::string	msg = ":" + irc.getHostname() + " 329 " + irc.getNickByFd(sender) + " " + chn + " :" + irc.channels[chn].getCreatedAtTime() + "\r\n";
		send(sender, msg.c_str(), msg.length(), MSG_DONTWAIT);
	}
}
