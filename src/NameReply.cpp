#include "../inc/ft_irc.hpp"

void    nameReply(Server &irc, std::string chn, int sender)
{
	if (irc.channels.find(chn) != irc.channels.end())
    {
		std::string				msg;
		std::string				clients = "";
		std::map<int, Client>	clientsMap = irc.channels[chn].members;

		for (std::map<int, Client>::iterator it = clientsMap.begin(); it != clientsMap.end(); ++it)
    	{
			if (it != clientsMap.begin())
				clients += " ";
        	if (irc.channels[chn].checkOperatorRole((it->first)))
        		clients += "@" + (it->second).getNickname();
        	else
            	clients += (it->second).getNickname();
    	}
		sendMessage(sender, RPL_NAMREPLY(irc.getHostname(), irc.getNickByFd(sender), chn, clients), ERR353);
		sendMessage(sender, RPL_ENDOFNAMES(irc.getHostname(), irc.getNickByFd(sender), chn), ERR366);
	}
}
