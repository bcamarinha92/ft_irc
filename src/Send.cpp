#include "../inc/ft_irc.hpp"
#include <string>

void    sendSequenceRPL(Server &irc, Message *message, int sender)
{
    std::string join;
    std::string nick = irc.getNickByFd(sender);
    std::string user = irc.getClientByFd(sender).getUsername();
    std::string hostClient = irc.getClientByFd(sender).getHostname();
    std::string hostServer = irc.getHostname();
    std::string realName = irc.getClientByFd(sender).getRealname();
    time_t t = irc.getCreationDate();

    if (join == "" || nick == "" || user == "" || hostClient == "" || hostServer == "" || realName == "")
        return ;
    (void)message;
    join = ":" + hostServer + " 001 " + nick + " :Welcome to the Paulo Brificado's IRC " + nick + "!\n";
    logConsole(join);
    send(sender, join.c_str(), join.length(), MSG_DONTWAIT);
    join = ":" + hostServer + " 002 " + nick + " :Your host is " + hostClient + ", running version 0.01\n";
    logConsole(join);
    send(sender, join.c_str(), join.length(), MSG_DONTWAIT);
    join = ":" + hostServer + " 003 " + nick + " :This server was created at " + std::string(std::ctime(&t)) + "\n";
    logConsole(join);
    send(sender, join.c_str(), join.length(), MSG_DONTWAIT);
    join = ":" + hostServer + " 004 " + nick + " :" + hostServer + " 0.01 ao itkol\n";
    logConsole(join);
    send(sender, join.c_str(), join.length(), MSG_DONTWAIT);
    join = ":" + hostServer + " 005 " + nick + " :PREFIX=(o)@ :CHANTYPES=# :CHANMODES=i,t,k,o,l :CASEMAPPING=ascii :TARGMAX=PART:1,NAMES:1,KICK:1,INVITE1:,WHO:1,WHOIS:1,PRIVMSG:1 :NETWORK=PauloBrificado :are supported by this server\r\n";
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

void	sendMessage(int fd, std::vector<int> fds, const std::string& msg, const std::string& emsg, bool all)
{
	std::string wholeMsg = msg + "\r\n";
    if(send(fd, wholeMsg.c_str(), wholeMsg.size(), MSG_DONTWAIT) < 0)
		std::cerr << emsg << std::endl;
	for(size_t i = 0; i < fds.size() && all; ++i)
	{
		if (fds[i] != fd)
			send(fds[i], wholeMsg.c_str(), wholeMsg.size(), MSG_DONTWAIT);
	}
}
