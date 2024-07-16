#include "../inc/ft_irc.hpp"


void sigHandler(int signal)
{
    if (signal == SIGINT)
        running = false;
    std::cout << running << std::endl;

}

void setNonBlocking(int socket)
{
    // int flags = fcntl(socket, F_GETFL, 0);
    // fcntl(socket, F_SETFL, flags | O_NONBLOCK);
    fcntl(socket, F_SETFL, O_NONBLOCK);
    int opt = 1;
    setsockopt(socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
}

void logConsole(std::string buffer)
{
    std::cout << buffer << std::endl;
}

void	sendMessage(int fd, const std::string& msg)
{
	std::string wholeMsg = msg + "\r\n";
    send(fd, msg.c_str(), msg.size(), MSG_DONTWAIT);
}

void	who(int sender, Server &irc, std::string const& chn)
{
	std::string const	msg;
	std::string			clients;

	for (std::map<int, Client>::iterator it = irc.channels[chn].)
	msg = ":hostcarol "+ irc.getNickByFd(sender) + " = " + chn + " :" +  + "\r\n";
	send(sender, msg.c_str(), msg.size(), MSG_DONTWAIT);
}

void broadcast(Server &irc, char *buffer, int sender)
{
    if (!strncmp(buffer, "NICK", 4))
    {
        irc.setNickByFd(sender, getNickFromBuffer(buffer));
        //send(sender, "RPL_WELCOME(bde-sous, bde-sous, localhost)", strlen("RPL_WELCOME(bde-sous, bde-sous, localhost)"),0);
    }
    //dei hardcode ao join para poder testar
    else if (!strncmp(buffer, "JOIN", 4))
    {
        std::string join = ":"+ irc.getNickByFd(sender) + " JOIN " + getChannelFromBuffer(buffer) + "\r\n";
        //std::string join = JOIN(irc.getNickByFd(sender),getChannelFromBuffer(buffer));
        send(sender, join.c_str(), join.length(),MSG_DONTWAIT);
		if (irc.channels.find(getChannelFromBuffer(buffer)) == irc.channels.end())
		{
			Channel	channel(getChannelFromBuffer(buffer));
			irc.addChannel(channel);
			irc.activateChannelMode(getChannelFromBuffer(buffer), 'n', sender, true);
			irc.activateChannelMode(getChannelFromBuffer(buffer), 't', sender, true);
			irc.channels[getChannelFromBuffer(buffer)].addClient(irc.getClientByFd(sender));
			irc.channels[getChannelFromBuffer(buffer)].addOperator(irc.getClientByFd(sender));
		}
		else
    		irc.channels[getChannelFromBuffer(buffer)].addClient(irc.getClientByFd(sender));
	}
    else
    {
        for (size_t i = 0; i < irc.pollfds.size(); ++i)
        {
            if ((irc.pollfds[i].fd == sender) || (irc.pollfds[i].fd == irc.getServerSocket()))
                continue;
            std::string join = ":"+ irc.getNickByFd(sender) + " " + std::string(buffer) + "\n";
            send(irc.pollfds[i].fd, join.c_str(), join.size(),MSG_DONTWAIT);
        }
    }
}

void closeFDs(Server &irc)
{
    std::cout << "closing fd" << std::endl;
    for (size_t i = 0; i < irc.pollfds.size(); ++i)
    {
        if (irc.pollfds[i].fd != irc.getServerSocket())
            irc.rmClient(irc.pollfds[i].fd, i);
    }
    std::cout << irc.pollfds.size() << std::endl;
    close(irc.getServerSocket());
}

void loopPool(Server &irc)
{
    char *buffer = 0;
    int bytesRead;
    int clientSocket;

    bytesRead = 0;
    clientSocket = 0;
    for (size_t i = 0; i < irc.pollfds.size(); ++i)
    {
        if (irc.pollfds[i].revents & POLLIN)
        {
            if (irc.pollfds[i].fd == irc.getServerSocket())
            {
                // Nova conexão detetada;
                Client user(irc.getServerSocket());
                irc.addClient(user);
                std::cout << "New connection from " << inet_ntoa(user.getclientAddr().sin_addr) << std::endl;
            }
            else
            {
                // Dados recebidos de um cliente com ligacao ja estabelecida previamente
                clientSocket = irc.pollfds[i].fd;
                //bytesRead = recv(clientSocket, buffer, sizeof(buffer),0);
                bytesRead = get_next_line(clientSocket, &buffer);
                if (bytesRead <= 0)
                {
                    //se 0 o fd fechou, se < 0 existe erro na leitura: em qualquer das situacoes o processo passa por dar disconnect
                    if (bytesRead < 0)
                        perror("read");
                    irc.rmClient(clientSocket, i);
                    std::cout << "Client disconnected" << std::endl;
                }
                else
                {
                    //leitura com sucesso, temos que inserir aqui o parsing e criar uma instancia da class Message.
                    //como ainda nao existe, simplesmente dou broadcast para todos os clientes ligados
                    broadcast(irc, buffer, clientSocket);
                    logConsole(std::string(buffer));
                }
            }
        }
    }
}

int main(int argc, char *argv[])
{
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " <port> <password>" << std::endl;
        return 1;
    }
    //falta parsing aos argumentos
    try
    {
        Server irc(atoi(argv[1]), argv[2]);
        int pollCount;
        signal(SIGINT, sigHandler);
        running = true;
        while (running)
        {
            pollCount = poll(irc.pollfds.data(), irc.pollfds.size(), -1);
            if (pollCount < 0)
                throw std::invalid_argument("poll");
            loopPool(irc);
        }
        closeFDs(irc);
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    return 0;
}
