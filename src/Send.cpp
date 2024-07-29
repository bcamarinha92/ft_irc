#include "../inc/ft_irc.hpp"

void    sendSequenceRPL(Server &irc, Message *message, int sender)
{
    std::string join;

    (void)message;
    join = ":" + irc.getHostname() + " 001 " + irc.getNickByFd(sender) + " :Welcome to the Paulo Brificado's IRC " + irc.getNickByFd(sender) + "!\n";
    logConsole(join);
    send(sender, join.c_str(), join.length(), MSG_DONTWAIT);
    join = ":" + irc.getHostname() + " 002 " + irc.getNickByFd(sender) + " :Your host is " + irc.getClientByFd(sender).getHostname() + ", running version 0.01\n";
    logConsole(join);
    send(sender, join.c_str(), join.length(), MSG_DONTWAIT);
    time_t t = irc.getCreationDate();
    join = ":" + irc.getHostname() + " 003 " + irc.getNickByFd(sender) + " :This server was created at " + std::string(std::ctime(&t)) + "\n";
    logConsole(join);
    send(sender, join.c_str(), join.length(), MSG_DONTWAIT);
    join = ":" + irc.getHostname() + " 004 " + irc.getNickByFd(sender) + " :" + irc.getHostname() + "0.01 ao itkol\n";
    logConsole(join);
    send(sender, join.c_str(), join.length(), MSG_DONTWAIT);
    join = ":" + irc.getHostname() + " 005 " + irc.getNickByFd(sender) + " :PREFIX=(ov)@+ CHANTYPES=#& CHANMODES=beI,k,l,imnprstz CASEMAPPING=ascii NETWORK=PauloBrificado\r\n";
    logConsole(join);
    send(sender, join.c_str(), join.length(), MSG_DONTWAIT);
}

//Esta funcao manda o MOTD vazio. Serve apenas para completar a ligacao e iniciar os PING/PONG
void    sendMOTD(Server &irc, Message *message, int sender)
{
    std::string join;

    (void)message;
    (void)irc;
    join = ":" + irc.getHostname() + " 422 " + irc.getNickByFd(sender) + " :MOTD File is missing\r\n";
    logConsole(join);
    send(sender, join.c_str(), join.length(), MSG_DONTWAIT);
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

void	sendMessageAll(int fd, std::vector<int> fds, const std::string& msg, const std::string& emsg)
{
	std::string	wholeMsg = msg + "\r\n";
	if(send(fd, wholeMsg.c_str(), wholeMsg.size(), MSG_DONTWAIT) < 0)
		std::cerr << emsg << std::endl;
	for(size_t i = 0; i < fds.size(); ++i)
	{
		if (fds[i] != fd)
			send(fds[i], wholeMsg.c_str(), wholeMsg.size(), MSG_DONTWAIT);
	}
}
