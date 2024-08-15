#include "../inc/ft_irc.hpp"

void    cmdWho(Server &irc, Message *message, int sender)
{
    std::string	chn = message->get_destination();

    if (chn.find("#") == std::string::npos)
    {
        int clientFd = irc.getFdFromNick(chn);

        if (clientFd == -1)
			return sendMessage(sender, ERR_NOSUCHNICK(irc.getHostname(), irc.getNickByFd(sender)), ERR401);
        Client 	&client = irc.getClientByFd(clientFd);
    	sendMessage(sender, RPL_WHOREPLY(irc.getHostname(), client.getNickname(), chn, client.getUsername(), \
									  client.getHostname(), client.getRealname()), ERR352);
	}
    else
    {
        Channel &channel = irc.channels[chn];

        if (channel.getName() == "")
            return sendMessage(sender, ERR_NOSUCHCHANNEL(irc.getHostname(), chn), ERR403);

		std::vector<int> clients = channel.getChannelClientsFds();
        for (std::vector<int>::iterator it = clients.begin(); it != clients.end(); ++it)
        {
            Client  &client = irc.getClientByFd(*it);
			sendMessage(sender, RPL_WHOREPLY(irc.getHostname(), client.getNickname(), chn, client.getUsername(), \
									client.getHostname(), client.getRealname()), ERR352);
        }
    }
	sendMessage(sender, RPL_ENDOFWHO(irc.getHostname(), irc.getNickByFd(sender), chn), ERR315);
}
