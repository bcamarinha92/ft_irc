#include "../inc/ft_irc.hpp"

void	cmdInvite(Server &irc, Message *message, int sender)
{
	std::string	user = "";
	std::string	chn = message->get_destination();

	if (message->get_parameters().size() > 1)
		user = message->get_parameters()[0];
	if (irc.channels.find(chn) == irc.channels.end())
		return sendMessage(sender, ERR_NOSUCHCHANNEL(irc.getHostname(), chn), ERR403);
	if (irc.channels[chn].members.find(sender) == irc.channels[chn].members.end())
		return sendMessage(sender, ERR_NOTONCHANNEL(irc.getHostname(), chn), ERR442);
	if (irc.clients.find(irc.getFdFromNick(user)) == irc.clients.end())
		return sendMessage(sender, ERR_NOSUCHNICK(irc.getHostname(), user), ERR401);
	if (irc.channels[chn].members.find(irc.getFdFromNick(user)) != irc.channels[chn].members.end())
		return sendMessage(sender, ERR_USERONCHANNEL(irc.getHostname(), user, chn), ERR443);
	if (!irc.channels[chn].checkOperatorRole(sender))
		return sendMessage(sender, ERR_CHANOPRIVSNEEDED(irc.getHostname(), irc.getNickByFd(sender), chn), ERR482);
	if (!irc.channels[chn].checkChannelUserInvite(irc.getFdFromNick(user)))
	{
		std::string	msg = ":" + irc.getNickByFd(sender) + "!" + irc.clients[sender].getUsername() \
			+ "@" + irc.getHostname() + " INVITE " + user + " " + chn;
		sendMessage(irc.getFdFromNick(user), msg.c_str(), ERR341);
		sendMessage(sender, RPL_INVITING(irc.getHostname(), irc.getNickByFd(sender), chn, user), ERR341);
		irc.channels[chn].addInvite(irc.getFdFromNick(user));
	}
}
