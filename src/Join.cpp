#include "../inc/ft_irc.hpp"

void    cmdJoin(Server &irc, Message *message, int sender)
{
    std::string					targets = message->get_destination();
    std::vector<std::string>    destinos = split(targets, ',');
	unsigned long      			i = 0;

	while (i < destinos.size())
	{
		logConsole(destinos[i]);
		std::string	t = destinos[i];
		if (t[0] != '#' || (t == "" && message->get_parameters().size() > 0))
			return (sendMessage(sender, ERR_NOSUCHCHANNEL(irc.getNickByFd(sender), t), ERR403));
		if (irc.clients[sender].channels.size() == CHANLIMIT)
			return sendMessage(sender, ERR_TOOMANYCHANNELS(irc.getHostname(), t), ERR405);

		if (!valid_channel(t, irc))
			return (sendMessage(sender, ERR_BADCHANMASK(irc.getNickByFd(sender), t), ERR476));
		Channel	channel(t);
		if (irc.channels.find(t) == irc.channels.end())
		{
			sendMessage(sender, JOIN(irc.getNickByFd(sender), t), ERRJ);
     		irc.addChannel(channel);
        	irc.channels[t].addClient(irc.getClientByFd(sender), irc);
	        irc.channels[t].addOperator(irc.getClientByFd(sender));
			if (irc.channels[t].getLaunch() == true)
				irc.channels[t].switchLaunch();
			irc.activateChannelMode(t, 'n', sender, true, "");
    	    irc.activateChannelMode(t, 't', sender, true, "");
            nameReply(irc,t,sender);
    	}
		else if (!irc.channels[t].checkChannelMode('l') ||
			irc.channels[t].members.size() < irc.channels[t].getChannelUserLimit())
		{
			if (!irc.channels[t].checkChannelMode('i') || irc.channels[t].checkChannelUserInvite(sender))
			{
				std::string	param = "";
				if (irc.channels[t].checkChannelMode('k') && message->get_parameters().size() > 1)
					param = message->get_parameters()[1];
				if (!irc.channels[t].checkChannelMode('k') || irc.channels[t].getChannelKey() == param)
				{
					sendMessageAll(sender, irc.channels[t].getChannelClientsFds(), \
				   	JOIN(irc.getNickByFd(sender), t), ERRJ, false);
					irc.channels[t].addClient(irc.getClientByFd(sender), irc);
					sendTopicJoin(irc, t, sender);
					nameReply(irc,t,sender);
				}
				else
					sendMessage(sender, ERR_BADCHANNELKEY(irc.getNickByFd(sender), t), ERR475);
			}
			else
				sendMessage(sender, ERR_INVITEONLYCHAN(irc.getNickByFd(sender), t), ERR473);
		}
		else
			sendMessage(sender, ERR_CHANNELISFULL(irc.getHostname(), irc.getNickByFd(sender), t), ERR471);
		i++;
	}
}
