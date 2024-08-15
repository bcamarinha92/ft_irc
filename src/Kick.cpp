#include "../inc/ft_irc.hpp"

void   cmdKick(Server &irc, Message *message, int sender)
{
    Channel				&channel = irc.channels[message->get_destination()];
    std::string			buffer = message->get_buffer();
    std::vector<int> 	fds = channel.getChannelClientsFds();
    std::string 		reason = "";
    int 				target;

    if (message->get_parameters().size() < 2)
        return sendMessage(sender, ERR_NEEDMOREPARAMS(irc.getHostname(), "KICK"), ERR461);
    target = irc.getFdFromNick(message->get_parameters()[1]);
    if (channel.getName().empty())
        return sendMessage(sender, ERR_NOSUCHCHANNEL(irc.getHostname(), \
													 channel.getName()), ERR403);
    if (std::find(fds.begin(), fds.end(), sender) == fds.end())
        return sendMessage(sender, \
						   ERR_USERNOTINCHANNEL(irc.getHostname(), irc.getNickByFd(sender), \
							  message->get_parameters()[1], channel.getName()), ERR441);
    if (std::find(fds.begin(), fds.end(), target) == fds.end())
        return sendMessage(sender, ERR_NOTONCHANNEL(irc.getHostname(), channel.getName()), ERR442);
    if (buffer.find(":") != std::string::npos)
        reason = buffer.substr(buffer.find(":") + 1);
    if (channel.checkOperatorRole(sender))
    {
        sendMessageAll(sender, channel.getChannelClientsFds(), \
					   KICK(irc.getHostname(), channel.getName(), \
			 message->get_parameters()[1], reason), ERRK, false);
        channel.rmClient(target, irc);
    }
    else
		sendMessage(sender, ERR_CHANOPRIVSNEEDED(irc.getHostname(), irc.getNickByFd(sender), \
										   channel.getName()), ERR482);
}
