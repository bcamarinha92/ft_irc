#include "../inc/ft_irc.hpp"

void    cmdNotice(Server &irc, Message *message, int sender)
{
	std::vector<std::string>    destinos = split(message->get_destination(), ',');
	//size_t						i = 0;

	if (message->get_parameters().size() < 2)
        return sendMessage(sender, ERR_NOTEXTTOSEND(irc.getNickByFd(sender)), ERR412);
	sendMessage(sender, NOTICE(irc.getNickByFd(sender), message->get_destination(), message->get_parameters()[1]), "");
	/*while (i < destinos.size())
    {
		std::cout << "destino: " << destinos[i] << std::endl;

        if (destinos[i][0] == '#')
        {
			if (irc.channels.find(destinos[i]) != irc.channels.end())
            {
				Channel &channel = irc.channels[destinos[i]];
				if (channel.members.find(sender) != channel.members.end())
					sendMessageAll(sender, channel.getChannelClientsFds(),
					NOTICE(irc.getNickByFd(sender), destinos[i], message->get_parameters()[1]), "", true);
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
				std::map<std::string, Channel>::iterator	it = irc.clients[irc.getFdFromNick(destinos[i])].channels.begin();
				for (; it != irc.clients[irc.getFdFromNick(destinos[i])].channels.end(); ++it)
					sendMessage(sender, NOTICE(irc.getNickByFd(sender), it->first, message->get_parameters()[1]), "");
                //std::string join = ":" + irc.getNickByFd(sender) + " " + message->get_command() + " " + destinos[i] + " :" + message->get_parameters()[1];
                //sendMessage(irc.getFdFromNick(destinos[i]), join, "");
            }
            else
                sendMessage(sender, ERR_NOSUCHNICK(irc.getNickByFd(sender), destinos[i]), ERR401);
        }
        i++;
    }*/
}
