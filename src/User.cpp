#include "../inc/ft_irc.hpp"

void    cmdUser(Server &irc, Message *message, int sender)
{
	//Depois de ter parsing ok deve ser efetuado o registo do username e realname aqui. no final se tudo correr bem, sao enviados os RPL e MOTD
	//The minimum length of <username> is 1, ie. it MUST NOT be empty. If it is empty,
	//the server SHOULD reject the command with ERR_NEEDMOREPARAMS (even if an empty parameter is provided);
	//otherwise it MUST use a default value instead.
	Client		&user = irc.getClientByFd(sender);
	std::string	a = user.getUsername();

	if (a != "")
		sendM(sender, ERR_ALREADYREGISTERED(irc.getHostname(), irc.getNickByFd(sender)), ERR462);
	else if (message->get_parameters().size() != 3)
		sendM(sender, ERR_NEEDMOREPARAMS(irc.getHostname(), message->get_command()), ERR461);
	else
	{
		std::string realname = parseRealname(message->get_buffer());
		if (realname == "")
			sendM(sender, ERR_NEEDMOREPARAMS(irc.getHostname(), message->get_command()), ERR461);
		else
		{
			std::string username = message->get_parameters()[0];

			user.setRealname(realname);
			if(username.length() > irc.MAX_LEN)
				username = username.substr(0, irc.MAX_LEN);
			user.setUsername(username);
			sendSequenceRPL(irc, message, sender);
			
		}
	}
}
