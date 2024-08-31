#include "../inc/ft_irc.hpp"
#include <vector>

void    cmdPing(Server &irc, Message *message, int sender)
{
    std::string	str = PING(irc.getHostname(), message->get_buffer());
    size_t		pos = str.find("PING");

    str.replace(pos, 4, "PONG");
    sendM(sender, str, ERRPIN);
}

void	cmdPong(Server &irc, Message *message, int sender)
{
    std::string	str = message->get_buffer();
    std::string	token;
	size_t		pos;
	Client&		user = irc.getClientByFd(sender);

    pos = str.find("PONG ");
	if (pos != std::string::npos)
		token = str.substr(pos + 5, str.find_last_not_of("\r\n") - pos - 4);
	if (token == irc.getHostname())
	{
		user.setLastAction();
		user.resetPingCount();
	}
}

void disconnectClient(Server &irc, Client& client, int i, std::string reason)
{
    std::map<std::string, Channel> aux = client.channels;

    for (std::map<std::string, Channel>::const_iterator it = aux.begin(); it != aux.end(); it++)
    {
        Channel& channel = irc.channels[it->first];
        std::vector<int> dest = channel.getChannelClientsFds();
        std::string str = ":" + client.getNickname() + " " + reason + " " + client.getNickname();
        sendMessageAll(client.getSocket(), dest, str, ERRNOT,1);
        channel.rmClient(client.getSocket(),irc);
    }
    std::string str = ":" + irc.getHostname() + " " + reason + " " + client.getNickname() + "\r\n";
    send(client.getSocket(), str.c_str(), str.size(), MSG_DONTWAIT);
    irc.rmClient(client.getSocket(), i);
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
                disconnectClient(irc, user, i, "QUIT :Ping timeout:");
                std::cout << "Client disconnected due to inactivity" << std::endl;
            }
            std::string join = ":" + irc.getHostname() + " PING " + irc.getHostname() + "\r\n";
            send(irc.pollfds[i].fd, join.c_str(), join.size(), MSG_DONTWAIT);
        }
    }
}
