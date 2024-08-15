#include "../inc/ft_irc.hpp"

void    cmdPrivMsg(Server &irc, Message *message, int sender)
{
	std::vector<std::string>    destinos = split(message->get_destination(), ',');
	size_t						i = 0;

	if (message->get_parameters().size() < 2)
        return sendMessage(sender, ERR_NOTEXTTOSEND(irc.getNickByFd(sender)), ERR412);
	while (i < destinos.size())
    {
        if (destinos[i][0] == '#')
        {
			if (irc.channels.find(destinos[i]) != irc.channels.end())
            {
				Channel &channel = irc.channels[destinos[i]];
				if (channel.members.find(sender) != channel.members.end())
					sendMessageAll(sender, channel.getChannelClientsFds(),
					PRIVMSG(irc.getNickByFd(sender), destinos[i], message->get_parameters()[1]), ERRPM, true);
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
                std::string join = ":" + irc.getNickByFd(sender) + " " + message->get_command() + " " + destinos[i] + " :" + message->get_parameters()[1];
                sendMessage(irc.getFdFromNick(destinos[i]), join, ERRPM);
            }
            else
                sendMessage(sender, ERR_NOSUCHNICK(irc.getNickByFd(sender), destinos[i]), ERR401);
        }
        i++;
    }
}
