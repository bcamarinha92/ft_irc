#include "../inc/ft_irc.hpp"

void    cmdNick(Server &irc, char *buffer, int sender)
{
    irc.setNickByFd(sender, getNickFromBuffer(buffer));
}

void    cmdJoin(Server &irc, char *buffer, int sender)
{
    std::string join = ":" + irc.getNickByFd(sender) + " JOIN " + getChannelFromBuffer(buffer) + "\r\n";
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
    if (irc.channels.find(getChannelFromBuffer(buffer)) != irc.channels.end())
    {
		std::string						msg;
		std::string						clients = "";
		std::map<int, Client>			clientsMap = irc.channels[getChannelFromBuffer(buffer)].members;
		for (std::map<int, Client>::iterator it = clientsMap.begin(); it != clientsMap.end(); ++it)
    	{
			if (it != clientsMap.begin())
				clients += " ";
        	if (irc.channels[getChannelFromBuffer(buffer)].checkOperatorRole((it->first)))
        		clients += "@" + (it->second).getNickname();
        	else
            	clients += (it->second).getNickname();
    	}
		logConsole("clientes: " + clients);
		msg = ":hostcarol 353 " + irc.getNickByFd(sender) + " = " + getChannelFromBuffer(buffer) + " :" + clients + "\r\n";
		send(sender, msg.c_str(), msg.size(), MSG_DONTWAIT);
	}
}

void	cmdMode(Server &irc, char *buffer, int sender)
{
	std::string	buf = getModeFromBuffer(buffer);
	std::cout << "buf: " << buf << std::endl;
	if (buf.size() > 1 && (!buf.compare(0, 1, "+", 0, 1) || !buf.compare(0, 1, "-", 0, 1)))
	{
		char	m = buf[1];
		if (!buf.compare(0, 1, "+", 0, 1))
			irc.channels[getChannelFromBuffer(buffer)].activateMode(m, sender, false);
		else
			irc.channels[getChannelFromBuffer(buffer)].deactivateMode(m, sender);
	}
	else if (buf == "\n")
	{
		std::cout << "CHANNEL: " << getChannelFromBuffer(buffer) << std::endl;
		std::string	msg = "MODE " + getChannelFromBuffer(buffer) + "\r\n";
		std::cout << msg << std::endl;
		send(sender, msg.c_str(), msg.size(), MSG_DONTWAIT);
	}
}
