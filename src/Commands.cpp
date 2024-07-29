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
    Channel	channel(chn);
	if (irc.channels.find(chn) == irc.channels.end())
	{
		sendMessage(sender, JOIN(irc.getNickByFd(sender), chn), ERR1);
        irc.addChannel(channel);
        irc.channels[chn].addClient(irc.getClientByFd(sender));
        irc.channels[chn].addOperator(irc.getClientByFd(sender));
		irc.clients[sender].addChannel(channel);
		if (irc.channels[chn].getLaunch() == true)
			irc.channels[chn].switchLaunch();
		irc.activateChannelMode(chn, 'n', sender, true, "");
        irc.activateChannelMode(chn, 't', sender, true, "");
    }
	else if (!irc.channels[chn].checkChannelMode('l') ||
		irc.channels[chn].members.size() < irc.channels[chn].getChannelUserLimit())
	{
		std::string	param = "";
		if (message->get_parameters().size() > 2)
			param = message->get_parameters()[2];
		if (!irc.channels[chn].checkChannelMode('k') || irc.channels[chn].getChannelKey() == param)
		{
			sendMessageAll(sender, irc.channels[chn].getChannelClientsFds(), \
				  JOIN(irc.getNickByFd(sender), chn), ERR1);
			irc.channels[chn].addClient(irc.getClientByFd(sender));
			irc.clients[sender].addChannel(channel);
		}
		else
			sendMessage(sender, ERR_BADCHANNELKEY(irc.getNickByFd(sender), chn), ERR19);
	}
	else
		sendMessage(sender, ERR_CHANNELISFULL(irc.getHostname(), irc.getNickByFd(sender), chn), ERR13);
}

void    cmdWho(Server &irc, Message *message, int sender)
{
	std::string	chn = message->get_destination();
	if (irc.channels.find(chn) != irc.channels.end())
    {
		std::string						msg;
		std::string						clients = "";
		std::map<int, Client>			clientsMap = irc.channels[chn].members;
		for (std::map<int, Client>::iterator it = clientsMap.begin(); it != clientsMap.end(); ++it)
    	{
			if (it != clientsMap.begin())
				clients += " ";
        	if (irc.channels[chn].checkOperatorRole((it->first)))
        		clients += "@" + (it->second).getNickname();
        	else
            	clients += (it->second).getNickname();
    	}
		sendMessage(sender, RPL_NAMREPLY(irc.getHostname(), irc.getNickByFd(sender), chn, clients), ERR8);
	}
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
    for (size_t i = 0; i < irc.pollfds.size(); ++i)
    {
        if ((irc.pollfds[i].fd == sender) || (irc.pollfds[i].fd == irc.getServerSocket()))
            continue;
		sendMessage(irc.pollfds[i].fd, PRIVMSG(irc.getNickByFd(sender), message->get_buffer()), ERR10);
    }
}

/*std::string getCurrentDateTime()
{
    // Get the current time
    std::time_t now = std::time(0);

    // Convert it to local time
    std::tm *localTime = std::localtime(&now);

    // Create a buffer to store the formatted date and time
    char buffer[100];

    // Format the date and time as "YYYY-MM-DD HH:MM:SS"
    std::strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", localTime);

    // Return the formatted date and time as a string
	std::cout << buffer << std::endl;
    return std::string(buffer);
}*/

void	cmdModeIterator(Server &irc, Message *message, int sender, std::string mode)
{
	std::string chn = message->get_destination();
	std::string	param = "";
	for (size_t i = 1, j = 1; i < mode.size(); i++)
	{
		char		m = mode[i];
		if ((irc.modesParam[m].first == 1 && mode[0] == '+') || (irc.modesParam[m].second == 1 && mode[0] == '-'))
			j++;
		if (message->get_parameters().size() > j)
			param = message->get_parameters()[j];
		if (!mode.compare(0, 1, "+", 0, 1))
			irc.activateChannelMode(chn, m, sender, false, param);
		else
			irc.deactivateChannelMode(chn, m, sender, param);
	}
}

void	cmdMode(Server &irc, Message *message, int sender)
{
	std::string	mode;
	if (message->get_parameters().size() > 1)
		mode = message->get_parameters()[1];
	std::string chn = message->get_destination();
	if (mode.size() > 1 && (!mode.compare(0, 1, "+", 0, 1) || !mode.compare(0, 1, "-", 0, 1)))
		cmdModeIterator(irc, message, sender, mode);
	else if (message->get_parameters().size() == 1 && !irc.channels[chn].getLaunch())
	{
		time_t  t = irc.channels[chn].getCreatedAtTime();
        std::stringstream ss;
        ss << t;
        std::cout << std::string(ctime(&t)) << std::endl;
        sendMessage(sender, RPL_CHANNELMODEIS(irc.getHostname(), irc.getNickByFd(sender), \
											  chn, irc.channels[chn].getChannelModes()), ERR2);
        sendMessage(sender, RPL_CREATIONTIME(irc.getHostname(), irc.getNickByFd(sender), chn, ss.str()), ERR3);
	}
}

void	cmdPart(Server &irc, Message *message, int sender)
{
	std::string chn = message->get_destination();
	std::string	reason = message->get_parameters()[1];
	std::string	msg = ":" + irc.getNickByFd(sender) + "!" + irc.clients[sender].getUsername() \
		+ "@" + irc.getHostname() + " PART " + chn + " " + reason;
	sendMessageAll(sender, irc.channels[chn].getChannelClientsFds(), msg, ERR6);
	irc.channels[chn].rmClient(sender);
	if (irc.channels[chn].checkOperatorRole(sender))
		irc.channels[chn].rmOperator(sender);
	irc.clients[sender].rmChannel(chn);
	if (irc.channels[chn].members.size() < 1)
		irc.rmChannel(chn);
}

void    cmdPing(Server &irc, Message *message, int sender)
{
    std::string join = ":" + irc.getHostname() + " ";
    join += message->get_buffer();
    size_t pos = join.find("PING");
    join.replace(pos, 4, "PONG");
    send(sender, join.c_str(), join.length(), MSG_DONTWAIT);
}

void cmdPong(Server &irc, Message *message, int sender)
{
    std::string join;
    std::string token;
	Client		user;
	size_t 		pos;

	user = irc.getClientByFd(sender);
    join += message->get_buffer();
    pos = join.find("PONG ");
    if (pos != std::string::npos) {
        token = join.substr(pos + 5); // Extract substring from pos + 5 to the end
    }
	if (token == irc.getHostname())
	{
		user.setLastAction();
		user.resetPingCount();
	}
}
