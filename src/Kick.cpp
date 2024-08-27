#include "../inc/ft_irc.hpp"

void   cmdKick(Server &irc, Message *message, int sender)
{
    Channel				        &channel = irc.channels[message->get_destination()];
    std::string			        buffer = message->get_buffer();
    std::vector<int> 	        fds = channel.getChannelClientsFds();
    std::string 		        reason = irc.getNickByFd(sender);
    std::vector<std::string>    target_nicks;
    int                         target;

    if (message->get_parameters().size() < 2)
        return sendMessage(sender, ERR_NEEDMOREPARAMS(irc.getHostname(), "KICK"), ERR461);
    target_nicks = split(message->get_parameters()[1], ',');
    for (size_t i = 0; i < target_nicks.size(); i++)
    {
        logConsole(target_nicks[i]);
        target = irc.getFdFromNick(target_nicks[i]);
        if (channel.getName().empty())
            return sendMessage(sender, ERR_NOSUCHCHANNEL(irc.getHostname(), \
                                                        channel.getName()), ERR403);
        if (std::find(fds.begin(), fds.end(), sender) == fds.end())
            return sendMessage(sender, \
                            ERR_USERNOTINCHANNEL(irc.getHostname(), irc.getNickByFd(sender), \
                                target_nicks[i], channel.getName()), ERR441);
        if (std::find(fds.begin(), fds.end(), target) == fds.end())
            return sendMessage(sender, ERR_NOTONCHANNEL(irc.getHostname(), channel.getName()), ERR442);
        if (message->get_parameters().size() == 3)
            reason = message->get_parameters()[2];
        if (channel.checkOperatorRole(sender))
        {
            sendMessageAll(sender, channel.getChannelClientsFds(), \
                        KICK(irc.getHostname(), channel.getName(), \
                target_nicks[i], reason), ERRK, false);
            channel.rmClient(target, irc);
        }
        else
            sendMessage(sender, ERR_CHANOPRIVSNEEDED(irc.getHostname(), irc.getNickByFd(sender), \
                                            channel.getName()), ERR482);
    }
    
}
