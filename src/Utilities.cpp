#include "../inc/ft_irc.hpp"

void closeFDs(Server &irc)
{
    size_t  i;

    i = 0;
    while (i < irc.pollfds.size())
    {
        if (irc.pollfds[i].fd != irc.getServerSocket())
            irc.rmClient(irc.pollfds[i].fd, i);
        i++;
    }
    close(irc.getServerSocket());
    irc.pollfds.erase(irc.pollfds.begin());
}

void sigHandler(int signal)
{
    if (signal == SIGINT)
        running = false;
}

void setNonBlocking(int socket)
{
    fcntl(socket, F_SETFL, O_NONBLOCK);
}

void    evaluatePing(Server &irc)
{
    time_t  currentTime = std::time(0);

    for (size_t i = 1; i < irc.pollfds.size(); ++i)
    {
        Client& user = irc.getClientByFd(irc.pollfds[i].fd);
        if (currentTime - user.getLastAction() > 30)
        {
            user.incPingCount();
            std::cout << "Ping Count " << user.getPingCount() << std::endl;
            if (user.getPingCount() == 3)
            {
                irc.rmClient(irc.pollfds[i].fd, i);
                //inserir aqui logica para remover o cliente dos canais em que estava
                std::cout << "Client disconnected due to inactivity" << std::endl;
            }
            std::string join = ":" + irc.getHostname() + " PING " + irc.getHostname() + "\r\n";
            send(irc.pollfds[i].fd, join.c_str(), join.size(), MSG_DONTWAIT);
        }
    }
}

std::string	toUpper(const std::string& str)
{
	std::string	res = str;
	for (size_t i = 0; i < res.length(); i++)
		res[i] = toupper(res[i]);
	return res;
}

int		aux(std::string targets)
{
	int	n = 1, i = 0;

	while (targets.find(",", i) != std::string::npos)
	{
		n++;
		i += targets.find(",", i) + 1;
	}
	return n;
}


std::vector<std::string> split(const std::string &s, char delim)
{
    std::vector<std::string> elems;
    std::stringstream ss(s);
    std::string item;
    while (std::getline(ss, item, delim))
        elems.push_back(item);
    return elems;
}
