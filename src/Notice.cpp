#include "../inc/ft_irc.hpp"

void    cmdNotice(Server &irc, Message *message, int sender)
{
	std::vector<std::string>    destinos = split(message->get_destination(), ',');
	size_t						i = 0;

	if (message->get_parameters().size() < 2)
        return sendMessage(sender, ERR_NOTEXTTOSEND(irc.getNickByFd(sender)), ERR412);
	while (i < destinos.size())
    {
		std::cout << "Msg: " << NOTICE(irc.getNickByFd(sender), irc.getHostname(), destinos[i], message->get_parameters()[1]) << std::endl;
        if (destinos[i][0] == '#')
        {
			if (irc.channels.find(destinos[i]) != irc.channels.end())
            {
				Channel &channel = irc.channels[destinos[i]];
				if (channel.members.find(sender) != channel.members.end())
					sendMessageAll(sender, channel.getChannelClientsFds(),
					NOTICE(irc.getNickByFd(sender), irc.getHostname(), destinos[i], message->get_parameters()[1]), ERRNOT, true);
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
				std::vector<int>	vec;
				vec.push_back(irc.getFdFromNick(destinos[i]));
				sendMessageAll(sender, vec, NOTICE(irc.getNickByFd(sender), irc.getHostname(), destinos[i], message->get_parameters()[1]), ERRNOT, true);
            }
            else
                sendMessage(sender, ERR_NOSUCHNICK(irc.getNickByFd(sender), destinos[i]), ERR401);
        }
        i++;
    }
}
