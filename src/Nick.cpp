#include "../inc/ft_irc.hpp"

void cmdNick(Server &irc, Message *message, int sender)
{
	if (message->get_parameters().size() == 1)
	{
		std::string nick = message->get_parameters()[0];
		if (!valid_nick(nick, irc, sender))
			return;

		if (irc.getNickByFd(sender) == "")
		{
			for (unsigned long i = 0; i < irc.pollfds.size(); i++)
			{
				if (irc.pollfds[i].fd == sender)
					continue;
				if (irc.getNickByFd(irc.pollfds[i].fd) == nick)
					return sendMessage(sender, ERR_NICKNAMEINUSE(irc.getHostname(), \
												  irc.getNickByFd(sender)), ERR433);
			}
			irc.setNickByFd(sender, nick);
			sendSequenceRPL(irc, message, sender);
		}
		else
		{
    		for (unsigned long i = 0; i < irc.pollfds.size(); i++)
    		{
    			if (irc.pollfds[i].fd == sender)
    				continue;
    			if (irc.getNickByFd(irc.pollfds[i].fd) == nick)
    				return sendMessage(sender, ERR_NICKNAMEINUSE(irc.getHostname(), \
    											  irc.getNickByFd(sender)), ERR433);
    		}
            Client client = irc.getClientByFd(sender);
            std::vector<int> dest = client.getUserChannelsFds(irc);
            std::string msg = NEWNICK(client.getNickname(),client.getHostname(), nick);
    		irc.setNickByFd(sender, nick);
            sendMessageAll(sender, dest, msg, ERRNOT,0);
		}
	}
	else
		sendMessage(sender, ERR_NONICKNAMEGIVEN(irc.getHostname()), ERR431);
}
