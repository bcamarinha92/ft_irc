#include "../inc/ft_irc.hpp"

void	cmdPart(Server &irc, Message *message, int sender)
{
	std::string chn = message->get_destination();
	//std::string	reason = "Leaving";
	std::string	reason = parseRealname(message->get_buffer());

	if (irc.channels.find(chn) == irc.channels.end())
		return sendMessage(sender, ERR_NOSUCHCHANNEL(irc.getHostname(), chn), ERR403);
	if (message->get_parameters().size() == 0)
		return sendMessage(sender, ERR_NEEDMOREPARAMS(irc.getHostname(), "PART"), ERR461);
	if (irc.channels[chn].members.find(sender) == irc.channels[chn].members.end())
		return sendMessage(sender, ERR_NOTONCHANNEL(irc.getHostname(), chn), ERR442);
	if (message->get_parameters().size() > 1)
		reason = message->get_parameters()[1];
	std::string	msg = ":" + irc.getNickByFd(sender) + "!" + irc.clients[sender].getUsername() \
		+ "@" + irc.getHostname() + " PART " + chn + " " + reason;
	sendMessageAll(sender, irc.channels[chn].getChannelClientsFds(), msg, ERRP, false);
	irc.channels[chn].rmClient(sender, irc);
	if (irc.channels[chn].members.size() < 1)
		irc.rmChannel(chn);
}
