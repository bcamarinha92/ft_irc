#include "../inc/ft_irc.hpp"

void    cmdPass(Server &irc, Message *message, int sender)
{
    std::string trash;

    if (message->get_parameters().size())
    {
        if (message->get_parameters()[0] != irc.getPassword())
        {
			sendMessage(sender, ERR_PASSWDMISMATCH(irc.getHostname()), ERR464);
            get_next_linepp(sender, trash , 1);
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
