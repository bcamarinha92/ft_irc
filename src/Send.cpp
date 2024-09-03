#include "../inc/ft_irc.hpp"
#include <stack>
#include <vector>

void sendMOTD(Server &irc, int fd)
{
    std::vector<std::string> motd;


    motd.push_back("Welcome to Paulo's Garage!");
    motd.push_back("        ______");
    motd.push_back("       //  ||\\ \\");
    motd.push_back(" _____//___||_\\ \\___");
    motd.push_back(" )  _          _    \\");
    motd.push_back(" |_/O\\________/O\\___|");
    motd.push_back("");
    motd.push_back("\\ \\        / / | |                          | |                     ");
    motd.push_back(" \\ \\  /\\  / /__| | ___ ___  _ __ ___   ___  | |_ ___                ");
    motd.push_back("  \\ \\/  \\/ / _ \\ |/ __/ _ \\| '_ ` _ \\ / _ \\ | __/ _ \\               ");
    motd.push_back("   \\  /\\  /  __/ | (_| (_) | | | | | |  __/ | || (_) |              ");
    motd.push_back(" ___\\/  \\/ \\___|_|\\___\\___/|_| |_| |_|\\___|  \\__\\___/               ");
    motd.push_back("|  __ \\          | |     ( )      / ____|                           ");
    motd.push_back("| |__) |_ _ _   _| | ___ |/ ___  | |  __  __ _ _ __ __ _  __ _  ___ ");
    motd.push_back("|  ___/ _` | | | | |/ _ \\  / __| | | |_ |/ _` | '__/ _` |/ _` |/ _ \\");
    motd.push_back("| |  | (_| | |_| | | (_) | \\__ \\ | |__| | (_| | | | (_| | (_| |  __/");
    motd.push_back("|_|   \\__,_|\\__,_|_|\\___/  |___/  \\_____|\\__,_|_|  \\__,_|\\__, |\\___|");
    motd.push_back("                                                          __/ |     ");
    motd.push_back("                                                         |___/ ");

    std::string motdStart = ":" + irc.getHostname() + " 375 " + irc.getNickByFd(fd) + " :MOTD -  Paulo's Garage";
    sendMessage(fd, motdStart, "Error sending MOTD");
    for (size_t i = 0; i < motd.size(); i++)
    {
        sendMessage(fd, motd[i], "Error sending MOTD");
    }
    motdStart = ":" + irc.getHostname() + " 376 " + irc.getNickByFd(fd) + " :End of /MOTD command.";
    sendMessage(fd, motdStart, "Error sending MOTD");
}

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
   	sendM(sender, RPL_CREATED(hostServer, nick, std::string(std::ctime(&t))), ERR003);
	sendMessage(sender, RPL_MYINFO(hostServer, nick), ERR004);
	sendMessage(sender, RPL_ISUPPORT(hostServer, nick), ERR005);
	sendMOTD(irc, sender);
	//Manda o MOTD vazio. Serve apenas para completar a ligacao e iniciar os PING/PONG
	//sendMessage(sender, ERR_NOMOTD(irc.getHostname(), irc.getNickByFd(sender)), ERR422);
	Client &client = irc.getClientByFd(sender);
	client.setAuthenticated();
}

void logConsole(std::string message)
{
    std::cout << message << std::endl;
}

void	sendM(int fd, const std::string& msg, const std::string& emsg)
{
	if(send(fd, msg.c_str(), msg.size(), MSG_DONTWAIT) < 0)
		std::cerr << emsg << std::endl;
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

void	sendPrivMsg(int fd, std::vector<int> fds, const std::string& msg, const std::string& emsg)
{
	std::string	wholeMsg = msg + "\r\n";

	for(size_t i = 0; i < fds.size(); ++i)
	{
		if (fds[i] != fd)
		    if(send(fds[i], wholeMsg.c_str(), wholeMsg.size(), MSG_DONTWAIT) < 0)
				std::cerr << emsg << std::endl;
	}
}
