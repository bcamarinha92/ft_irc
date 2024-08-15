#include "../inc/ft_irc.hpp"

void    cmdCap(Server &irc, Message *message, int sender)
{
	if (message->get_buffer().find("CAP LS") != std::string::npos)
		sendMessage(sender, CAP(irc.getNickByFd(sender)), ERRCAP);
}
