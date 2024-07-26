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