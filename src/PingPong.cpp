#include "../inc/ft_irc.hpp"

void    cmdPing(Server &irc, Message *message, int sender)
{
    std::string	str = PING(irc.getHostname(), message->get_buffer());
    size_t		pos = str.find("PING");

    str.replace(pos, 4, "PONG");
    sendM(sender, str, ERRPIN);
}

void	cmdPong(Server &irc, Message *message, int sender)
{
    std::string	str = message->get_buffer();
    std::string	token;
	size_t		pos;
	Client&		user = irc.getClientByFd(sender);

    pos = str.find("PONG ");
	if (pos != std::string::npos)
		token = str.substr(pos + 5, str.find_last_not_of("\r\n") - pos - 4);
	if (token == irc.getHostname())
	{
		user.setLastAction();
		user.resetPingCount();
	}
}
