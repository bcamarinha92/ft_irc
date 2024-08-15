#include "../inc/ft_irc.hpp"
#include <cstring>

void    cmdPass(Server &irc, Message *message, int sender)
{
    char   **trash = NULL;

    if (message->get_parameters().size())
    {
        if (message->get_parameters()[0] != irc.getPassword())
        {
			sendMessage(sender, ERR_PASSWDMISMATCH(irc.getHostname()), ERR464);
            get_next_line(sender, trash , 1);
            irc.rmClient(sender);
        }
        else
        {
            Client& client = irc.getClientByFd(sender);
            client.setPwdStatus();
        }
    }
    else
		sendMessage(sender, ERR_NEEDMOREPARAMS(irc.getHostname(), message->get_command()), ERR461);
}

