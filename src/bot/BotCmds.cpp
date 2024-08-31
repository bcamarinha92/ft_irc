#include "../../inc/BotClient.hpp"

void	cmdPass(BotClient& b)
{
	std::string passCmd = "PASS " + b.getPassword() + "\r\n";
    if (send(b.getSocket(), passCmd.c_str(), passCmd.length(), 0) < 0)
		std::cerr << "Error: sending PASS message" << std::endl;
}

void	cmdNick(BotClient& b)
{
	std::string nickCmd = "NICK " + b.getNickname() + "\r\n";
	if (send(b.getSocket(), nickCmd.c_str(), nickCmd.length(), 0) < 0)
		std::cerr << "Error: sending NICK message" << std::endl;
}

void	cmdUser(BotClient& b)
{
	std::string userCmd = "USER " + b.getUsername() + " " + b.getNickname() + \
		" bla :" + b.getRealname() + "\r\n";
	send(b.getSocket(), userCmd.c_str(), userCmd.length(), 0);
}

void	cmdPing(std::vector<std::string> elems, size_t i, BotClient& b)
{
	if (elems.size() > i + 1)
	{
		std::string	str = "PONG " + elems[i + 1];
		if(send(b.getSocket(), str.c_str(), str.size(), MSG_DONTWAIT) < 0)
			std::cerr << "Error: sending PING message" << std::endl;
	}
}

int	generateRandomNumber()
{
    return std::rand() % 10;
}

void	cmdPrivMsg(std::vector<std::string> elems, size_t i, std::string buffer, BotClient& b)
{
	std::vector<std::string>	buf = split(buffer, ':');

	if (buf.size() > 1)
	{
		if (buf[2].find(b.getNickname()) != std::string::npos)
		{
			int			n = generateRandomNumber();

			std::string	msgCmd = "PRIVMSG " + elems[i + 1] + " :" + b.msgs[n] + "\r\n";
			send(b.getSocket(), msgCmd.c_str(), msgCmd.length(), 0);
		}
	}
}

void	cmdInvite(std::vector<std::string> elems, size_t i, BotClient& b)
{
	if (elems.size() > i + 2)
	{
		std::string	nick = elems[i + 1];
		std::string	chn = elems[i + 2];
		std::cout << "Nick: " << nick << " Chn: " << chn << std::endl;
		if (chn[0] == '#')
		{
			std::string joinCmd = "JOIN " + chn + "\r\n";
			std::cout << b.getSocket() << std::endl;
    		send(b.getSocket(), joinCmd.c_str(), joinCmd.length(), 0);
		}
	}
}
