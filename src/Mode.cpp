#include "../inc/ft_irc.hpp"

void	cmdModeIterator(Server &irc, Message *message, int sender, std::string mode)
{
	std::string chn = message->get_destination();
	std::string	param = "";

	for (size_t i = 1, j = 0; i < mode.size(); i++)
	{
		char	m = mode[i];
		if ((irc.modesParam[m].first == 1 && mode[0] == '+')
			|| (irc.modesParam[m].second == 1 && mode[0] == '-'))
			j++;
		if (message->get_parameters().size() > j + 1)
			param = message->get_parameters()[j + 1];
		if (!mode.compare(0, 1, "+", 0, 1))
			irc.activateChannelMode(chn, m, sender, false, param);
		else
			irc.deactivateChannelMode(chn, m, sender, param);
	}
}

void	cmdMode(Server &irc, Message *message, int sender)
{
	std::string	mode;
	std::string chn = message->get_destination();

	if (message->get_parameters().size() > 1)
		mode = message->get_parameters()[1];
	if (mode.size() > 1 && (!mode.compare(0, 1, "+", 0, 1) || !mode.compare(0, 1, "-", 0, 1)))
		cmdModeIterator(irc, message, sender, mode);
	else if (message->get_parameters().size() == 1 && !irc.channels[chn].getLaunch())
	{
		time_t  t = irc.channels[chn].getCreatedAtTime();
        std::stringstream ss;
        ss << t;
        sendMessage(sender, RPL_CHANNELMODEIS(irc.getHostname(), irc.getNickByFd(sender), \
											  chn, irc.channels[chn].getChannelModes()), ERRM);
        sendMessage(sender, RPL_CREATIONTIME(irc.getHostname(), irc.getNickByFd(sender), \
											 chn, ss.str()), ERRC);
	}
}
