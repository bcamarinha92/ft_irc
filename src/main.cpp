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

void broadcast(Server &irc, char *buffer, int sender)
{
    if (!strncmp(buffer, "NICK", 4))
    {
        irc.setNickByFd(sender, getNickFromBuffer(buffer));
        //send(sender, "RPL_WELCOME(bde-sous, bde-sous, localhost)", strlen("RPL_WELCOME(bde-sous, bde-sous, localhost)"),0);
    }
    else if (!strncmp(buffer, "JOIN", 4))
    {
        std::string join = ":"+ irc.getNickByFd(sender) + " JOIN " + getChannelFromBuffer(buffer) + "\n";
        //std::string join = JOIN(irc.getNickByFd(sender),getChannelFromBuffer(buffer));
        send(sender, join.c_str(), join.length(),MSG_DONTWAIT);
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
    if (!strncmp(buffer, "QUIT", 4))
    {
        running = false;
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
