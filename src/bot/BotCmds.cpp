#include "../../inc/BotClient.hpp"

void	sendM(int fd, const std::string& msg, const std::string& emsg)
{
	if(send(fd, msg.c_str(), msg.size(), MSG_DONTWAIT) < 0)
		std::cerr << emsg << std::endl;
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
