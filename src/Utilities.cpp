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
        if (currentTime - user.getLastAction() > (30 * (user.getPingCount() + 1)))
        {
            user.incPingCount();
            std::cout << "Ping Count " << user.getPingCount() << std::endl;
            if (user.getPingCount() == 4)
            {
                irc.rmClient(irc.pollfds[i].fd, i);
                //inserir aqui logica para remover o cliente dos canais em que estava
				std::map<std::string, Channel>::iterator	it;
				for (it = irc.clients[irc.pollfds[i].fd].channels.begin(); it != irc.clients[irc.pollfds[i].fd].channels.end(); ++it)
					irc.channels[it->first].rmClient(irc.pollfds[i].fd, irc);
                std::cout << "Client disconnected due to inactivity" << std::endl;
            }
			sendMessage(irc.pollfds[i].fd, PING(irc.getHostname()), ERRPIN);
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

std::vector<std::string> split(const std::string &s, char delim)
{
    std::vector<std::string> elems;
    std::stringstream ss(s);
    std::string item;
    while (std::getline(ss, item, delim))
        elems.push_back(item);
    return elems;
}

std::string	cleanString(const std::string& name)
{
    std::string	cleanedStr;

    for (std::string::const_iterator it = name.begin(); it != name.end(); ++it)
	{
        if (std::isprint(*it))
            cleanedStr += *it;
    }
    return cleanedStr;
}
