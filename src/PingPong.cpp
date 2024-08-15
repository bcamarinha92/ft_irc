#include "../inc/ft_irc.hpp"

void    cmdPing(Server &irc, Message *message, int sender)
{
    std::string	join = ":" + irc.getHostname() + " ";

    join += message->get_buffer();
    size_t	pos = join.find("PING");
    join.replace(pos, 4, "PONG");
    send(sender, join.c_str(), join.length(), MSG_DONTWAIT);
}

void	cmdPong(Server &irc, Message *message, int sender)
{
    std::string	join;
    std::string	token;
	size_t		pos;
	Client&		user = irc.getClientByFd(sender);

    join += message->get_buffer();
    pos = join.find("PONG ");
	if (pos != std::string::npos)
		token = join.substr(pos + 5, join.find_last_not_of("\r\n") - pos - 4);
	if (token == irc.getHostname())
	{
		user.setLastAction();
		user.resetPingCount();
	}
}
