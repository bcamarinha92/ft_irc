#include "../inc/ft_irc.hpp"


void sigHandler(int signal) 
{
    if (signal == SIGINT) 
        running = false;
}

std::string extractNickFromInput(const std::string& input) 
{
    std::istringstream iss(input);
    std::string line;
    
    while (std::getline(iss, line)) 
    {
        if (line.substr(0, 5) == "NICK ")
            return line.substr(5);
    }
    return "";
}

void setNonBlocking(int socket) 
{
    int flags = fcntl(socket, F_GETFL, 0);
    fcntl(socket, F_SETFL, flags | O_NONBLOCK);
}

void logConsole(char *buffer)
{
    std::cout << buffer << std::endl;
}

void broadcast(Server &irc, char *buffer, int sender)
{
    if (!strncmp(buffer, "CAP", 3))
    {
        irc.setNickByFd(sender, extractNickFromInput(buffer));
        std::cout << "nick " << irc.getNickByFd(sender) <<std::endl;
    }
    //dei hardcode ao join para poder testar 
    if (!strncmp(buffer, "JOIN", 4))
    {
        std::string join = ":"+ irc.getNickByFd(sender) + " " + std::string(buffer) + "\n";
        std::cout << join << std::endl;
        write (sender, join.c_str(), join.length());
    }
    for (size_t i = 0; i < irc.pollfds.size(); ++i)
    {
        if ((irc.pollfds[i].fd == sender) || (irc.pollfds[i].fd == irc.getServerSocket()))
            continue;
        write(irc.pollfds[i].fd, buffer, strlen(buffer));
    }
}

void closeFDs(Server &irc)
{
    for (size_t i = 0; i < irc.pollfds.size(); ++i)
    {
        if (irc.pollfds[i].fd != irc.getServerSocket())
            irc.rmClient(irc.pollfds[i].fd, i);
    } 
}

void loopPool(Server &irc)
{
    char buffer[BUFFER_SIZE];
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
                bytesRead = read(clientSocket, buffer, sizeof(buffer));
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
                    buffer[bytesRead] = '\0';
                    broadcast(irc, buffer, clientSocket);
                    logConsole(buffer);
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
        close(irc.getServerSocket());
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    return 0;
}
