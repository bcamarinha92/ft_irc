#include "../inc/ft_irc.hpp"

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
					return sendMessage(sender, ERR_NICKNAMEINUSE(irc.getHostname(), \
												  irc.getNickByFd(sender)), ERR433);
			}
			irc.setNickByFd(sender, nick);
			sendSequenceRPL(irc, message, sender);
		}
		else
			return sendMessage(sender, ERR_NICKNAMEINUSE(irc.getHostname(), \
												irc.getNickByFd(sender)), ERR433);
	}
	else
		sendMessage(sender, ERR_NONICKNAMEGIVEN(irc.getHostname()), ERR431);
}
