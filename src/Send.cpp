#include "../inc/ft_irc.hpp"

void    sendSequenceRPL(Server &irc, Message *message, int sender)
{
    std::string	nick = irc.getNickByFd(sender);
    std::string	user = irc.getClientByFd(sender).getUsername();
    std::string	hostClient = irc.getClientByFd(sender).getHostname();
    std::string	hostServer = irc.getHostname();
    std::string	realName = irc.getClientByFd(sender).getRealname();
    time_t t = irc.getCreationDate();

    if (nick == "" || user == "" || hostClient == "" || hostServer == "" || realName == "")
        return ;
    (void)message;
	sendMessage(sender, RPL_WELCOME(hostServer, nick), ERR001);
	sendMessage(sender, RPL_YOURHOST(hostServer, nick, hostClient), ERR002);
   	sendMessage(sender, RPL_CREATED(hostServer, nick, std::string(std::ctime(&t))), ERR003);
	sendMessage(sender, RPL_MYINFO(hostServer, nick), ERR004);
	sendMessage(sender, RPL_ISUPPORT(hostServer, nick), ERR005);
	//Manda o MOTD vazio. Serve apenas para completar a ligacao e iniciar os PING/PONG
	sendMessage(sender, ERR_NOMOTD(irc.getHostname(), irc.getNickByFd(sender)), ERR422);
}

void logConsole(std::string message)
{
    std::cout << message << std::endl;
}

void	sendMessage(int fd, const std::string& msg, const std::string& emsg)
{
	std::string wholeMsg = msg + "\r\n";

    if(send(fd, wholeMsg.c_str(), wholeMsg.size(), MSG_DONTWAIT) < 0)
		std::cerr << emsg << std::endl;
}

void	sendMessageAll(int fd, std::vector<int> fds, const std::string& msg, const std::string& emsg, bool pm)
{
	std::string	wholeMsg = msg + "\r\n";

	if (!pm)
		if (send(fd, wholeMsg.c_str(), wholeMsg.size(), MSG_DONTWAIT) < 0)
			std::cerr << emsg << std::endl;
	for(size_t i = 0; i < fds.size(); ++i)
	{
		if (fds[i] != fd)
			send(fds[i], wholeMsg.c_str(), wholeMsg.size(), MSG_DONTWAIT);
	}
}
