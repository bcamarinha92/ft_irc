#include "../inc/ft_irc.hpp"

void   cmdTopic(Server &irc, Message *message, int sender)
{
    Channel 			&channel = irc.channels[message->get_destination()];
    std::string 		buffer = message->get_buffer();
    std::vector<int>	fds = channel.getChannelClientsFds();

    if (channel.getName().empty())
		return sendMessage(sender, ERR_NOSUCHCHANNEL(irc.getHostname(), channel.getName()), ERR403);
    if (std::find(fds.begin(), fds.end(), sender) == fds.end())
		return sendMessage(sender, ERR_NOTONCHANNEL(irc.getHostname(), channel.getName()), ERR442);
    if (buffer.find(":") != std::string::npos)
    {
        std::string topic = buffer.substr(buffer.find(":") + 1);
        if (channel.checkChannelMode('t') && channel.checkOperatorRole(sender))
        {
            channel.setTopic(topic);
			sendMessageAll(sender, channel.getChannelClientsFds(), \
				  RPL_TOPIC(irc.getHostname(), irc.getNickByFd(sender), channel.getName(), \
				channel.getTopic()), ERR332, false);
        }
        else
        {
            if (!channel.checkOperatorRole(sender))
				sendMessage(sender, ERR_CHANOPRIVSNEEDED(irc.getHostname(), irc.getNickByFd(sender), \
											 channel.getName()), ERR482);
            else if (!channel.checkChannelMode('t'))
				sendMessage(sender, ERR_NOTMODE(irc.getHostname(), irc.getNickByFd(sender), \
									channel.getName()), ERRNTM);
        }
    }
    else
    {
        if (channel.getTopic().size() > 0)
			sendMessage(sender, RPL_TOPIC(irc.getHostname(), irc.getNickByFd(sender), channel.getName(), \
								 channel.getTopic()), ERR332);
        else
			sendMessage(sender, RPL_NOTOPIC(irc.getHostname(), irc.getNickByFd(sender), \
								   channel.getName()), ERR331);
    }
}
