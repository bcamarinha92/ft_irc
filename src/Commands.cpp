#include "../inc/ft_irc.hpp"
#include <string>

void    cmdCap(Server &irc, Message *message, int sender)
{
    std::string	str;
    (void)irc;
    if (message->get_buffer().find("CAP REQ")!= std::string::npos)
        str = ":bde-sous CAP ACK :multi-prefix\r\n";
    else if (message->get_buffer().find("CAP LS")!= std::string::npos)
        str = ":bde-sous CAP * LS :multi-prefix\r\n";
    else
    {
        sendSequenceRPL(irc, message, sender);
        sendMOTD(irc, message, sender);
        return ;
    }
    send(sender, str.c_str(), str.length(), MSG_DONTWAIT);
}

void    cmdNick(Server &irc, Message *message, int sender)
{
    irc.setNickByFd(sender, getNickFromBuffer(message->get_buffer()));
    std::cout << "Registered user " << irc.getNickByFd(sender) << std::endl;
}

void    cmdJoin(Server &irc, Message *message, int sender)
{
    std::string	targets = message->get_destination();
	int			i = 0;
	bool		stop = false;

	while (true)
	{
		std::cout << "i1: " << i << std::endl;
		std::cout << "find: " << targets.find(",", i) << std::endl;
		std::string	t = targets.substr(i, (targets.find(",", i) != std::string::npos ? targets.find(",", i) : targets.length()) - i);
		std::cout << "t: " << t << std::endl;
		if (t == "")
			if (message->get_parameters().size() > 0)
				return (sendMessage(sender, ERR_NOSUCHCHANNEL(irc.getNickByFd(sender), t), ERR403));
    	Channel	channel(t);
		if (irc.channels.find(t) == irc.channels.end())
		{
			if (irc.clients[sender].channels.size() == 10)
				return sendMessage(sender, ERR_TOOMANYCHANNELS(irc.getHostname(), t), ERR405);
			sendMessage(sender, JOIN(irc.getNickByFd(sender), t), ERRJ);
     		irc.addChannel(channel);
        	irc.channels[t].addClient(irc.getClientByFd(sender));
	        irc.channels[t].addOperator(irc.getClientByFd(sender));
			irc.clients[sender].addChannel(channel);
			if (irc.channels[t].getLaunch() == true)
				irc.channels[t].switchLaunch();
			irc.activateChannelMode(t, 'n', sender, true, "");
    	    irc.activateChannelMode(t, 't', sender, true, "");
    	}
		else if (!irc.channels[t].checkChannelMode('l') ||
			irc.channels[t].members.size() < irc.channels[t].getChannelUserLimit())
		{
			std::string	param = "";
			if (irc.channels[t].checkChannelMode('k') && message->get_parameters().size() > 0)
				param = message->get_parameters()[0];
			if (!irc.channels[t].checkChannelMode('k') || irc.channels[t].getChannelKey() == param)
			{
				if (irc.clients[sender].channels.size() == 10)
					return sendMessage(sender, ERR_TOOMANYCHANNELS(irc.getHostname(), t), ERR405);
				sendMessageAll(sender, irc.channels[t].getChannelClientsFds(), \
				   JOIN(irc.getNickByFd(sender), t), ERRJ);
				irc.channels[t].addClient(irc.getClientByFd(sender));
				irc.clients[sender].addChannel(channel);
			}
			else
				sendMessage(sender, ERR_BADCHANNELKEY(irc.getNickByFd(sender), t), ERR475);
		}
		else
			sendMessage(sender, ERR_CHANNELISFULL(irc.getHostname(), irc.getNickByFd(sender), t), ERR471);
		std::cout << "i2: " << i << std::endl;
		if (stop)
			break;
		i = (targets.find(",", i) != std::string::npos ? targets.find(",", i) : targets.length());
		if (targets.find(",", i++) == std::string::npos)
			stop = true;
	}
}

void    cmdWho(Server &irc, Message *message, int sender)
{
	std::string	chn = message->get_destination();
	std::cout << chn << std::endl;
	if (irc.channels.find(chn) != irc.channels.end())
    {
		std::string								msg;
		std::string								clients = "";
		std::map<int, Client>					clientsMap = irc.channels[chn].members;
		for (std::map<int, Client>::iterator	it = clientsMap.begin(); it != clientsMap.end(); ++it)
    	{
			if (it != clientsMap.begin())
				clients += " ";
        	if (irc.channels[chn].checkOperatorRole((it->first)))
        		clients += "@" + (it->second).getNickname();
        	else
            	clients += (it->second).getNickname();
    	}
		sendMessage(sender, RPL_NAMREPLY(irc.getHostname(), irc.getNickByFd(sender), chn, clients), ERR353);
	}
}

void    cmdPass(Server &irc, Message *message, int sender)
{
    if (message->get_parameters().size() == 1)
    {
        if (message->get_parameters()[0] != irc.getPassword())
        {
			sendMessage(sender, ERR_PASSWDMISMATCH(irc.getHostname()), ERR464);
            close(sender);
        }
    }
    else
		sendMessage(sender, ERR_NEEDMOREPARAMS(irc.getHostname(), message->get_command()), ERR461);
}

void    cmdPrivMsg(Server &irc, Message *message, int sender)
{
	std::string					targets = message->get_destination();
	std::string					ftargets = "";
	std::vector<std::string>	vtargets;
	size_t						i = 0;

	while (true)
	{
		std::string	t = targets.substr(i, (targets.find(",", i) != std::string::npos ? targets.find(",", i) : targets.length()));
		if (irc.getFdFromNick(t) > 0)
		{
			ftargets += t;
			vtargets.push_back(t);
		}
		else
			sendMessage(sender, ERR_NOSUCHNICK(irc.getHostname(), t), ERR401);
		i += (targets.find(",", i) != std::string::npos ? targets.find(",", i) : targets.length());
		if (targets.find(",", i++) == std::string::npos)
			break;
	}
	if (message->get_parameters().size() > 0)
	{
		std::string				msg = message->get_parameters()[0];
		std::string::size_type	nbytes = message->get_parameters()[0].size();
		if (nbytes > 512)
			sendMessage(sender, ERR_INPUTTOOLONG(irc.getHostname()), ERR417);
		else
		{
			for (i = 0; i < vtargets.size(); i++)
    		{
        		if (irc.getFdFromNick(vtargets[i]) == sender)
            		continue;
				sendMessage(irc.getFdFromNick(vtargets[i]), \
				PRIVMSG(irc.getNickByFd(sender), ftargets, msg), ERRPM);
    		}
		}
	}
	else
		sendMessage(sender, ERR_NOTEXTTOSEND(irc.getHostname()), ERR412);
}

void	cmdModeIterator(Server &irc, Message *message, int sender, std::string mode)
{
	std::string chn = message->get_destination();
	std::string	param = "";

	for (size_t i = 1, j = 0; i < mode.size(); i++)
	{
		char	m = mode[i];
		if ((irc.modesParam[m].first == 1 && mode[0] == '+')
			|| (irc.modesParam[m].second == 1 && mode[0] == '-'))
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
	std::string chn = message->get_destination();

	if (message->get_parameters().size() > 0)
		mode = message->get_parameters()[0];
	if (mode.size() > 1 && (!mode.compare(0, 1, "+", 0, 1) || !mode.compare(0, 1, "-", 0, 1)))
		cmdModeIterator(irc, message, sender, mode);
	else if (message->get_parameters().size() == 0 && !irc.channels[chn].getLaunch())
	{
		time_t  t = irc.channels[chn].getCreatedAtTime();
        std::stringstream ss;
        ss << t;
        std::cout << std::string(ctime(&t)) << std::endl;
        sendMessage(sender, RPL_CHANNELMODEIS(irc.getHostname(), irc.getNickByFd(sender), \
											  chn, irc.channels[chn].getChannelModes()), ERRM);
        sendMessage(sender, RPL_CREATIONTIME(irc.getHostname(), irc.getNickByFd(sender), chn, ss.str()), ERRC);
	}
}

void	cmdPart(Server &irc, Message *message, int sender)
{
	std::string chn = message->get_destination();
	if (irc.channels.find(chn) == irc.channels.end())
		return sendMessage(sender, ERR_NOSUCHCHANNEL(irc.getHostname(), chn), ERR403);
	if (message->get_parameters().size() == 0)
		return sendMessage(sender, ERR_NEEDMOREPARAMS(irc.getHostname(), message->get_command()), ERR461);
	if (irc.channels[chn].members.find(sender) == irc.channels[chn].members.end())
		return sendMessage(sender, ERR_NOTONCHANNEL(irc.getHostname(), chn), ERR442);
	std::string	reason = message->get_parameters()[0];
	std::string	msg = ":" + irc.getNickByFd(sender) + "!" + irc.clients[sender].getUsername() \
		+ "@" + irc.getHostname() + " PART " + chn + " " + reason;
	sendMessageAll(sender, irc.channels[chn].getChannelClientsFds(), msg, ERRP);
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

void	cmdPong(Server &irc, Message *message, int sender)
{
    std::string	join;
    std::string	token;
	Client		user;
	size_t		pos;

	user = irc.getClientByFd(sender);
    join += message->get_buffer();
    pos = join.find("PONG ");
	if (pos != std::string::npos)
		token = join.substr(pos + 5); // Extract substring from pos + 5 to the end
	if (token == irc.getHostname())
	{
		user.setLastAction();
		user.resetPingCount();
	}
}
