#include "../inc/ft_irc.hpp"

void    sendSequenceRPL(Server &irc, Message *message, int sender)
{
    std::string join;

    (void)message;
    join = ":localhost 001 bde-sous :Welcome to the Paulo Brificado's IRC " + irc.getNickByFd(sender) + "!\n"; 
    logConsole(join);
    send(sender, join.c_str(), join.length(), MSG_DONTWAIT);
    join = ":localhost 002 bde-sous :Your host is PauloBrificado, running version 0.01\n";
    logConsole(join);
    send(sender, join.c_str(), join.length(), MSG_DONTWAIT);
    time_t t = irc.getCreationDate();
    join = ":localhost 003 bde-sous :This server was created at " + std::string(std::ctime(&t)) + "\n";
    logConsole(join);
    send(sender, join.c_str(), join.length(), MSG_DONTWAIT);
    join = ":localhost 004 bde-sous :localhost 0.01 ao itkol\n";
    logConsole(join);
    send(sender, join.c_str(), join.length(), MSG_DONTWAIT);
    join = ":localhost 005 bde-sous :PREFIX=(ov)@+ CHANTYPES=#& CHANMODES=beI,k,l,imnprstz CASEMAPPING=ascii NETWORK=PauloBrificado\r\n";
    logConsole(join);
    send(sender, join.c_str(), join.length(), MSG_DONTWAIT);
}

//Esta funcao manda o MOTD vazio. Serve apenas para completar a ligacao e iniciar os PING/PONG
void    sendMOTD(Server &irc, Message *message, int sender)
{
    std::string join;

    (void)message;
    (void)irc;
    join = ":localhost 422 bde-sous :MOTD File is missing\r\n";
    logConsole(join);
    send(sender, join.c_str(), join.length(), MSG_DONTWAIT);
}

void logConsole(std::string message)
{
    std::cout << message << std::endl;
}

void	sendMessage(int fd, const std::string& msg)
{
	std::string wholeMsg = msg + "\r\n";
    send(fd, msg.c_str(), msg.size(), MSG_DONTWAIT);
}
