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

void	who(int sender, Server &irc, std::string const& chn, bool op)
{
    (void)op;
	std::string	msg;
	std::string	clients;
	std::map<int, Client>	clientsMap = irc.channels[chn].getChannelClients(false);
	std::map<int, Client>::iterator it = clientsMap.begin();
	clients = irc.getNickByFd(it->first);
	//++it;
	//for (; it != clientsMap.end(); ++it)
	//	clients += " " + irc.getNickByFd(it->first);;
	logConsole("clientes: " + clients);
	msg = ":hostcarol 353 " + irc.getNickByFd(sender) + " = " + chn + " :@csilva-f\r\n";
	send(sender, msg.c_str(), msg.size(), MSG_DONTWAIT);
	//std::string	msg2 = ":" + irc.getNickByFd(sender) + " " + chn + " :End of /NAMES list.\r\n";
	//send(sender, msg2.c_str(), msg2.size(), MSG_DONTWAIT);
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
        send(sender, join.c_str(), join.length(),MSG_DONTWAIT);
		if (irc.channels.find(getChannelFromBuffer(buffer)) == irc.channels.end())
		{
			Channel	channel(getChannelFromBuffer(buffer));
			irc.addChannel(channel);
			irc.activateChannelMode(getChannelFromBuffer(buffer), 'n', sender, true);
			irc.activateChannelMode(getChannelFromBuffer(buffer), 't', sender, true);
			irc.channels[getChannelFromBuffer(buffer)].addClient(irc.getClientByFd(sender));
			who(sender, irc, channel.getName(), true);
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
            if (pollCount < 0 && running) 
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
