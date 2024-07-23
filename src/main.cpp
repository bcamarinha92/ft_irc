#include "../inc/ft_irc.hpp"
#include <iostream>

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

void	sendMessage(int fd, const std::string& msg)
{
	std::string wholeMsg = msg + "\r\n";
    send(fd, msg.c_str(), msg.size(), MSG_DONTWAIT);
}

void broadcast(Server &irc, Message *message, int sender)
{
    if (message->get_command() == "PASS")
        cmdPass(irc, message, sender);
    else if (message->get_command() == "NICK")
        cmdNick(irc, message, sender);
    else if (message->get_command() == "JOIN")
        cmdJoin(irc, message, sender);
    else if (message->get_command() == "WHO")
        cmdWho(irc,message->get_destination(),sender);
    else if (message->get_command() == "PRIVMSG") {
        cmdPrivMsg(irc,message,sender);
	} else if (message->get_command() == "CAP") {
		cmdCap(message);
	} else if (message->get_command() == "USER") {
		cmdUser(message,&irc.getClientByFd(sender));
	}
    // else
    // {
    //     std::string join = ":server 461 :Not enough parameters\n";
    //     logConsole(join);
    //     send(sender, join.c_str(), join.length(), MSG_DONTWAIT);
    // }
	std::cout << *message << std::endl;
}

void	who(int sender, Server &irc, std::string const& chn, bool op)
{
    (void)op;
	std::string	msg;
    std::string clients = "";
	std::map<int, Client>	clientsMap = irc.channels[chn].members;
	std::map<int, Client>::iterator it = clientsMap.begin();
	for (; it != clientsMap.end(); ++it)
    {
        if (irc.channels[chn].checkOperatorRole((it->first)))
        	clients += " @" + (it->second).getNickname();
        else
            clients += " " + (it->second).getNickname();
    }
	logConsole("clientes: " + clients);
	msg = ":hostcarol 353 " + irc.getNickByFd(sender) + " = " + chn + " :" + clients + "\r\n";
	send(sender, msg.c_str(), msg.size(), MSG_DONTWAIT);
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
    char *message = 0;
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
                bytesRead = get_next_line(clientSocket, &message);
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
                    Message new_message(message, clientSocket, irc.getNickByFd(clientSocket));
                    broadcast(irc, &new_message, clientSocket);
                    //broadcast(irc, message, clientSocket);
                    logConsole(std::string(message));
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
