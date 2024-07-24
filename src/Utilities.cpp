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
    // int flags = fcntl(socket, F_GETFL, 0);
    // fcntl(socket, F_SETFL, flags | O_NONBLOCK);
    fcntl(socket, F_SETFL, O_NONBLOCK);
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
