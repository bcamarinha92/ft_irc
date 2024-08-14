#include "../inc/ft_irc.hpp"

void    cmdCap(Server &irc, Message *message, int sender)
{
    std::string join;
    (void)irc;

	if (message->get_buffer().find("CAP LS")!= std::string::npos)
	{
        join = ":" + irc.getNickByFd(sender) + " CAP * LS :\r\n";
    	send(sender, join.c_str(), join.length(), MSG_DONTWAIT);
	}
}

void cmdNick(Server &irc, Message *message, int sender)
{
	if (message->get_parameters().size() == 1)
	{
		std::string nick = message->get_parameters()[0];

		if (irc.getNickByFd(sender) == "")
		{
			for (unsigned long i = 0; i < irc.pollfds.size(); i++)
			{
				if (irc.pollfds[i].fd == sender)
					continue;
				if (irc.getNickByFd(irc.pollfds[i].fd) == nick)
				{
					std::string join = ":" + irc.getHostname() + " 433 " + irc.getNickByFd(sender) + " :Nickname is already in use\n";
					logConsole(join);
					send(sender, join.c_str(), join.length(), MSG_DONTWAIT);
					return;
				}
			}
			irc.setNickByFd(sender, nick);
			sendSequenceRPL(irc, message, sender);
		}
		else
		{
			std::string join = ":" + irc.getHostname() + " 433 " + irc.getNickByFd(sender) + " :Nickname is already set\n";
			logConsole(join);
			send(sender, join.c_str(), join.length(), MSG_DONTWAIT);
			return;
		}
	}
	else
	{
		std::string join = ":" + irc.getHostname() + " 431 * :No nickname given\n";
		logConsole(join);
		send(sender, join.c_str(), join.length(), MSG_DONTWAIT);
	}
}

void    cmdJoin(Server &irc, Message *message, int sender)
{
    std::string	targets = message->get_destination();
    std::vector<std::string>    destinos = split(targets, ',');
	unsigned long      i = 0;

	while (i < destinos.size())
	{
		std::string	t = destinos[i];
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
            nameReply(irc,t,sender);
    	}
		else if (!irc.channels[t].checkChannelMode('l') ||
			irc.channels[t].members.size() < irc.channels[t].getChannelUserLimit())
		{
			std::string	param = "";
			if (irc.channels[t].checkChannelMode('k') && message->get_parameters().size() > 1)
				param = message->get_parameters()[1];
			if (!irc.channels[t].checkChannelMode('k') || irc.channels[t].getChannelKey() == param)
			{
				if (irc.clients[sender].channels.size() == 10)
					return sendMessage(sender, ERR_TOOMANYCHANNELS(irc.getHostname(), t), ERR405);
				sendMessageAll(sender, irc.channels[t].getChannelClientsFds(), \
				   JOIN(irc.getNickByFd(sender), t), ERRJ);
				irc.channels[t].addClient(irc.getClientByFd(sender));
				irc.clients[sender].addChannel(channel);
				nameReply(irc,t,sender);
			}
			else
				sendMessage(sender, ERR_BADCHANNELKEY(irc.getNickByFd(sender), t), ERR475);
		}
		else
			sendMessage(sender, ERR_CHANNELISFULL(irc.getHostname(), irc.getNickByFd(sender), t), ERR471);
		i++;
	}
}

void    cmdPrivMsg(Server &irc, Message *message, int sender)
{
	std::vector<std::string>    destinos = split(message->get_destination(), ',');
	size_t						i = 0;
	std::cout << *message << std::endl;
	while (i < destinos.size())
    {
        if (message->get_parameters().size() == 0)
            return sendMessage(sender, ERR_NOTEXTTOSEND(irc.getNickByFd(sender)), ERR412);
        if (destinos[i][0] == '#')
        {
			if (irc.channels.find(destinos[i]) != irc.channels.end())
            {
				Channel &channel = irc.channels[destinos[i]];
				if (channel.members.find(sender) != channel.members.end())
				{
					std::string join = ":" + irc.getNickByFd(sender) + " " +message->get_command() + " " + destinos[i] + " :" + message->get_parameters()[1];
					sendPrivMsg(sender, channel.getChannelClientsFds(), join, ERRPM);
				}
				else
                    sendMessage(sender, ERR_NOTONCHANNEL(irc.getHostname(), destinos[i]), ERR442);
			}
			else
				sendMessage(sender, ERR_NOSUCHNICK(irc.getNickByFd(sender), destinos[i]), ERR401);
        }
        else
        {
            if (irc.clients.find(irc.getFdFromNick(destinos[i])) != irc.clients.end())
            {
                std::string join = ":" + irc.getNickByFd(sender) + " " +message->get_command() + " " + destinos[i] + " :" + message->get_parameters()[1];
                sendMessage(irc.getFdFromNick(destinos[i]), join, ERRPM);
            }
            else
                sendMessage(sender, ERR_NOSUCHNICK(irc.getNickByFd(sender), destinos[i]), ERR401);
        }
        i++;
    }
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
		if (message->get_parameters().size() > j + 1)
			param = message->get_parameters()[j + 1];
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

	if (message->get_parameters().size() > 1)
		mode = message->get_parameters()[1];
	if (mode.size() > 1 && (!mode.compare(0, 1, "+", 0, 1) || !mode.compare(0, 1, "-", 0, 1)))
		cmdModeIterator(irc, message, sender, mode);
	else if (message->get_parameters().size() == 1 && !irc.channels[chn].getLaunch())
	{
		time_t  t = irc.channels[chn].getCreatedAtTime();
        std::stringstream ss;
        ss << t;
        sendMessage(sender, RPL_CHANNELMODEIS(irc.getHostname(), irc.getNickByFd(sender), \
											  chn, irc.channels[chn].getChannelModes()), ERRM);
        sendMessage(sender, RPL_CREATIONTIME(irc.getHostname(), irc.getNickByFd(sender), chn, ss.str()), ERRC);
	}
}

void	cmdPart(Server &irc, Message *message, int sender)
{
	std::string chn = message->get_destination();
	std::string	reason = "Leaving";
	if (irc.channels.find(chn) == irc.channels.end())
		return sendMessage(sender, ERR_NOSUCHCHANNEL(irc.getHostname(), chn), ERR403);
	if (message->get_parameters().size() == 0)
		return sendMessage(sender, ERR_NEEDMOREPARAMS(irc.getHostname(), message->get_command()), ERR461);
	if (irc.channels[chn].members.find(sender) == irc.channels[chn].members.end())
		return sendMessage(sender, ERR_NOTONCHANNEL(irc.getHostname(), chn), ERR442);
	if (message->get_parameters().size() > 1)
		reason = message->get_parameters()[1];
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

void    nameReply(Server &irc, std::string chn, int sender)
{
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
        std::string join = ":" + irc.getHostname() + " 366 " + irc.getNickByFd(sender) + " " + chn + " :End of /Names list.\r\n";
        send(sender, join.c_str(), join.length(), MSG_DONTWAIT);
	}
}

void    cmdWho(Server &irc, Message *message, int sender)
{
    std::string chn = message->get_destination();
    std::string join;

    if (chn.find("#") == std::string::npos)
    {
        int clientFd = irc.getFdFromNick(chn);

        if (clientFd == -1)
        {
            std::string join = ":" + irc.getHostname() + " 401 " + irc.getNickByFd(sender) + " " + chn + " :No such nick/channel\r\n";
            send(sender, join.c_str(), join.length(), MSG_DONTWAIT);
            return ;
        }
        Client  &client = irc.getClientByFd(clientFd);
        join = ":" + irc.getHostname() + " 352 " + client.getNickname() + " " + \
            message->get_destination() + " " + client.getUsername() + " " + client.getHostname()+ \
            " " + irc.getHostname() + " " + client.getNickname() + " H@ :0 " + client.getRealname() + "";
        send(sender, join.c_str(), join.length(), MSG_DONTWAIT);
    }
    else
    {
        Channel &channel = irc.channels[chn];

        if (channel.getName() == "")
        {
            std::string join = ":" + irc.getHostname() + " 403 " + irc.getNickByFd(sender) + " " + chn + " :No such channel\r\n";
            send(sender, join.c_str(), join.length(), MSG_DONTWAIT);
            return ;
        }
        std::vector<int> clients = channel.getChannelClientsFds();
        for (std::vector<int>::iterator it = clients.begin(); it != clients.end(); ++it)
        {
            Client  &client = irc.getClientByFd(*it);
            join = ":" + irc.getHostname() + " 352 " + client.getNickname() + " " + \
                message->get_destination() + " " + client.getUsername() + " " + client.getHostname()+ \
                " " + irc.getHostname() + " " + client.getNickname() + " H@ :0 " + client.getRealname() + "";
            send(sender, join.c_str(), join.length(), MSG_DONTWAIT);
        }
    }
    join = ":" + irc.getHostname() + " 315 " + irc.getNickByFd(sender) + " " + message->get_destination() + " :End of /WHO list.\r\n";
    send(sender, join.c_str(), join.length(), MSG_DONTWAIT);
}

void    cmdPass(Server &irc, Message *message, int sender)
{
    char   **trash = NULL;
    if (message->get_parameters().size())
    {
        if (message->get_parameters()[0] != irc.getPassword())
        {
            std::string join = ":" + irc.getHostname() + " 464 : Password Mismatch\r\n";
            logConsole(join);
            send(sender, join.c_str(), join.length(), MSG_DONTWAIT);
            get_next_line(sender, trash , 1);
            irc.rmClient(sender);
        }
        else
        {
            Client& client = irc.getClientByFd(sender);
            client.setPwdStatus();
        }
    }
    else
    {
        std::string join = ":" + irc.getHostname() + " 461 :Not enough parameters\n";
        logConsole(join);
        send(sender, join.c_str(), join.length(), MSG_DONTWAIT);
    }
}

void   cmdTopic(Server &irc, Message *message, int sender)
{
    Channel &channel = irc.channels[message->get_destination()];
    std::string buffer = message->get_buffer();
    std::vector<int> fds = channel.getChannelClientsFds();

    if (channel.getName().empty())
    {
        std::string join = ":" + irc.getHostname() + " 403 " + irc.getNickByFd(sender) + " " + message->get_destination() + " :No such channel\n";
        send(sender, join.c_str(), join.length(), MSG_DONTWAIT);
        return;
    }
    if (std::find(fds.begin(), fds.end(), sender) == fds.end())
    {
        std::string join = ":" + irc.getHostname() + " 442 " + irc.getNickByFd(sender) + " " + channel.getName() + " :You're not on that channel\n";
        send(sender, join.c_str(), join.length(), MSG_DONTWAIT);
        return;
    }
    if (buffer.find(":") != std::string::npos)
    {
        std::string topic = buffer.substr(buffer.find(":") + 1);
        if (channel.checkChannelMode('t') && channel.checkOperatorRole(sender))
        {
            channel.setTopic(topic);
            std::string join = ":" + irc.getHostname() + " 332 " + irc.getNickByFd(sender) + " " + channel.getName() + " :" + channel.getTopic() + "\n";
            sendMessage(sender, channel.getChannelClientsFds(), join, "a",true);
        }
        else
        {
            if (!channel.checkOperatorRole(sender))
            {
                std::string join = ":" + irc.getHostname() + " 482 " + irc.getNickByFd(sender) + " " + channel.getName() + " :You're not channel operator\n";
                send(sender, join.c_str(), join.length(), MSG_DONTWAIT);
            }
            else if (!channel.checkChannelMode('t'))
            {
                std::string join = ":" + irc.getHostname() + " 482 " + irc.getNickByFd(sender) + " " + channel.getName() + " :Channel is not set as topic\n";
                send(sender, join.c_str(), join.length(), MSG_DONTWAIT);
            }
        }
    }
    else
    {
        if (channel.getTopic().size() > 0)
        {
            std::string join = ":" + irc.getHostname() + " 332 " + irc.getNickByFd(sender) + " " + channel.getName() + " :" + channel.getTopic() + "\n";
            send(sender, join.c_str(), join.length(), MSG_DONTWAIT);
        }
        else
        {
            std::string join = ":" + irc.getHostname() + " 331 " + irc.getNickByFd(sender) + " " + channel.getName() + " :No topic is set\n";
            send(sender, join.c_str(), join.length(), MSG_DONTWAIT);
        }
    }
}

void   cmdKick(Server &irc, Message *message, int sender)
{
    Channel &channel = irc.channels[message->get_destination()];
    std::string buffer = message->get_buffer();
    std::vector<int> fds = channel.getChannelClientsFds();
    std::string reason = "";
    int target;

    if (message->get_parameters().size() < 2)
    {
        std::string join = ":" + irc.getHostname() + " 461 " + irc.getNickByFd(sender) + " KICK :Not enough parameters\n";
        send(sender, join.c_str(), join.length(), MSG_DONTWAIT);
        return;
    }
    target = irc.getFdFromNick(message->get_parameters()[1]);
    if (channel.getName().empty())
    {
        std::string join = ":" + irc.getHostname() + " 403 " + irc.getNickByFd(sender) + " " + message->get_destination() + " :No such channel\n";
        send(sender, join.c_str(), join.length(), MSG_DONTWAIT);
        return;
    }
    if (std::find(fds.begin(), fds.end(), sender) == fds.end())
    {
        std::string join = ":" + irc.getHostname() + " 441 " + irc.getNickByFd(sender) + " " + message->get_parameters()[1] + " " + channel.getName() + " :You're not on that channel\n";
        send(sender, join.c_str(), join.length(), MSG_DONTWAIT);
        return;
    }
    if (std::find(fds.begin(), fds.end(), target) == fds.end())
    {
        std::string join = ":" + irc.getHostname() + " 442 " + irc.getNickByFd(sender) + " " + channel.getName() + " :They aren't on that channel\n";
        send(sender, join.c_str(), join.length(), MSG_DONTWAIT);
        return;
    }
    if (buffer.find(":") != std::string::npos)
        reason = buffer.substr(buffer.find(":") + 1);
    if (channel.checkOperatorRole(sender))
    {
        std::string join = ":" + irc.getHostname() + " KICK " + channel.getName() + " " + message->get_parameters()[1] + " :" + reason + "\n";
        sendMessage(sender, channel.getChannelClientsFds(), join, "a",true);
        channel.rmClient(target);
    }
    else
    {
        std::string join = ":" + irc.getHostname() + " 482 " + irc.getNickByFd(sender) + " " + channel.getName() + " :You're not channel operator\n";
        send(sender, join.c_str(), join.length(), MSG_DONTWAIT);
    }
}

void    cmdUser(Server &irc, Message *message, int sender)
{
	//Depois de ter parsing ok deve ser efetuado o registo do username e realname aqui. no final se tudo correr bem, sao enviados os RPL e MOTD
	//The minimum length of <username> is 1, ie. it MUST NOT be empty. If it is empty,
	//the server SHOULD reject the command with ERR_NEEDMOREPARAMS (even if an empty parameter is provided);
	//otherwise it MUST use a default value instead.
	Client &user = irc.getClientByFd(sender);
	std::string a = user.getUsername();
	if (a != "")
	{
		std::string join = ERR_ALREADYREGISTERED(irc.getHostname(),irc.getNickByFd(sender));
		send(sender, join.c_str(),join.size(),MSG_DONTWAIT);
	}
	else
	{
		a = message->get_parameters()[0];
		user.setRealname(parseRealname(message->get_buffer()));
		user.setUsername(message->get_parameters()[0]);
		sendSequenceRPL(irc, message, sender);
	}
}
