#include "../inc/ft_irc.hpp"

void    cmdCap(Server &irc, Message *message, int sender)
{
	(void)irc;
	(void)message;
	(void)sender;
    /*std::string	msg;
    (void)irc;
    if (message->get_buffer().find("CAP REQ")!= std::string::npos)
        msg = ":bde-sous CAP ACK :multi-prefix\r\n";
    else if (message->get_buffer().find("CAP LS")!= std::string::npos)
        msg = ":bde-sous CAP * LS :multi-prefix\r\n";
    else
        return ;
    send(sender, msg.c_str(), msg.length(), MSG_DONTWAIT);*/
}

void    cmdNick(Server &irc, Message *message, int sender)
{
    irc.setNickByFd(sender, getNickFromBuffer(message->get_buffer()));
    /*std::cout << "Registered user " << irc.getNickByFd(sender) << std::endl;
    std::string msg = ":localhost 002 bde-sous :Your host is PauloBrificado, running version 0.01\n";
    logConsole(msg);
    send(sender, msg.c_str(), msg.length(), MSG_DONTWAIT);
    time_t t = irc.getCreationDate();
    msg = ":localhost 003 bde-sous :This server was created at " + std::string(std::ctime(&t)) + "\n";
    logConsole(msg);
    send(sender, msg.c_str(), msg.length(), MSG_DONTWAIT);
    msg = ":localhost 004 bde-sous localhost 0.01 ao itkol\n";
    logConsole(msg);
    send(sender, msg.c_str(), msg.length(), MSG_DONTWAIT);
    msg = ":localhost 005 bde-sous PREFIX=(ov)@+ CHANTYPES=#& CHANMODES=beI,k,l,imnprstz CASEMAPPING=ascii NETWORK=PauloBrificado\r\n";
    logConsole(msg);
    send(sender, msg.c_str(), msg.length(), MSG_DONTWAIT);
    msg = ":localhost 372 bde-sous :- Welcome to the IRC Server - Enjoy your stay!\r\n";
    logConsole(msg);
    send(sender, msg.c_str(), msg.length(), MSG_DONTWAIT);*/
}

void    cmdJoin(Server &irc, Message *message, int sender)
{
    std::string chn = message->get_destination();
    Channel	channel(chn);
	if (irc.channels.find(chn) == irc.channels.end())
	{
		sendMessage(sender, irc.channels[chn].getChannelClientsFds(), \
			  JOIN(irc.getNickByFd(sender), chn), ERR1, false);
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
		(irc.channels[chn].members.size() < irc.channels[chn].getChannelUserLimit() \
		&& irc.channels[chn].getChannelUserLimit() > 0))
	{
		sendMessage(sender, irc.channels[chn].getChannelClientsFds(), \
			  JOIN(irc.getNickByFd(sender), chn), ERR1, true);
		irc.channels[chn].addClient(irc.getClientByFd(sender));
		irc.clients[sender].addChannel(channel);
	}
	else
		sendMessage(sender, irc.channels[chn].getChannelClientsFds(), \
			  ERR_CHANNELISFULL(irc.getHostname(), irc.getNickByFd(sender), chn), ERR13, false);
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
		sendMessage(sender, irc.channels[chn].getChannelClientsFds(), \
			  RPL_NAMREPLY(irc.getHostname(), irc.getNickByFd(sender), chn, clients), ERR8, false);
	}
}

void    cmdPass(Server &irc, Message *message, int sender)
{
    if (message->get_parameters().size() == 1)
    {
        if (message->get_parameters()[0] == irc.getPassword())
        {
            std::string msg = ":server 001 :Welcome to the Paulo Brificado's IRC " \
				+ irc.getNickByFd(sender) + "!\n"; // Está a sempre comer a primeira palavra idk why
            logConsole(msg);
            send(sender, msg.c_str(), msg.length(), MSG_DONTWAIT);
        }
        else
        {
            std::string msg = ":server 403 " + irc.getNickByFd(sender) + " :Invalid password\n";
            logConsole(msg);
            send(sender, msg.c_str(), msg.length(), MSG_DONTWAIT);
            close(sender);
        }
    }
    else
    {
        std::string msg = ":server 461 :Not enough parameters\n";
        logConsole(msg);
        send(sender, msg.c_str(), msg.length(), MSG_DONTWAIT);
    }
}

void    cmdPrivMsg(Server &irc, Message *message, int sender)
{
    for (size_t i = 0; i < irc.pollfds.size(); ++i)
    {
        if ((irc.pollfds[i].fd == sender) || (irc.pollfds[i].fd == irc.getServerSocket()))
            continue;
		sendMessage(irc.pollfds[i].fd, irc.channels[message->get_destination()].getChannelClientsFds(), \
			  PRIVMSG(irc.getNickByFd(sender), message->get_buffer()), ERR10, false);
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
	for (size_t i = 1, j = 1; i < mode.size(); i++)
	{
		char		m = mode[i];
		if ((irc.modesParam[m].first == 1 && mode[0] == '+') || (irc.modesParam[m].second == 1 && mode[0] == '-'))
			j++;
		std::string param = message->get_parameters()[j];
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
		time_t	t = irc.channels[chn].getCreatedAtTime();
		sendMessage(sender, irc.channels[chn].getChannelClientsFds(), \
			  RPL_CHANNELMODEIS(irc.getHostname(), irc.getNickByFd(sender), \
					   chn, irc.channels[chn].getChannelModes()), ERR2, false);
		sendMessage(sender, irc.channels[chn].getChannelClientsFds(), \
			  RPL_CREATIONTIME(irc.getHostname(), irc.getNickByFd(sender), \
					  chn, std::string(ctime(&t))), ERR3, false);
	}
}

void	cmdPart(Server &irc, Message *message, int sender)
{
	std::string chn = message->get_destination();
	std::string	reason = message->get_parameters()[1];
	std::string	msg = ":" + irc.getNickByFd(sender) + "!" + irc.clients[sender].getUsername() \
		+ "@" + irc.getHostname() + " PART " + chn + " " + reason;
	sendMessage(sender, irc.channels[chn].getChannelClientsFds(), msg, ERR6, true);
	irc.channels[chn].rmClient(sender);
	if (irc.channels[chn].checkOperatorRole(sender))
		irc.channels[chn].rmOperator(sender);
	irc.clients[sender].rmChannel(chn);
	if (irc.channels[chn].members.size() < 1)
		irc.rmChannel(chn);
}
