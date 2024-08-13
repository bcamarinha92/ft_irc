#include "../inc/ft_irc.hpp"

void broadcast(Server &irc, Message *message, int sender)
{
    Client &client = irc.getClientByFd(sender);

    if (client.getPwdStatus() == false)
    {
        if (message->get_command() == "CAP")
            cmdCap(irc, message, sender);
        else if (message->get_command() == "PASS")
            cmdPass(irc, message, sender);
        else
        {
            std::string join = ":" + irc.getHostname() + " 451 " + irc.getNickByFd(sender) + " :You have not registered\r\n";
            logConsole(join);
            send(sender, join.c_str(), join.length(), MSG_DONTWAIT);
        }
        return;
    }
    if (message->get_command() == "PING")
        cmdPing(irc, message, sender);
    else if (message->get_command() == "PONG")
        cmdPong(irc, message, sender);
	else if (message->get_command() == "CAP")
        cmdCap(irc, message, sender);
    else if (message->get_command() == "USER")
		cmdUser(irc, message, sender);
	else if (message->get_command() == "NICK")
		cmdNick(irc, message, sender);
	else if (message->get_command() == "JOIN")
		cmdJoin(irc, message, sender);
	else if (message->get_command() == "WHO")
		cmdWho(irc, message, sender);
	else if (message->get_command() == "MODE")
		cmdMode(irc, message, sender);
	else if (message->get_command() == "PRIVMSG")
		cmdPrivMsg(irc, message, sender);
	else if (message->get_command() == "PART")
		cmdPart(irc, message, sender);
	else if (message->get_command() == "TOPIC")
	   cmdTopic(irc, message, sender);
	else if (message->get_command() == "KICK")
	   cmdKick(irc, message, sender);
    client.setLastAction();
}

void	loopPool(Server &irc)
{
    char *message = 0;
    int bytesRead = 0;
    int clientSocket = 0;

    for (size_t i = 0; i < irc.pollfds.size(); ++i)
    {
        if (irc.pollfds[i].revents & POLLIN)
        {
            if (irc.pollfds[i].fd == irc.getServerSocket())
            {
                // Nova conexão detetada;
                Client user(irc.getServerSocket());
                irc.addClient(user);
                std::cout << "New connection from " << user.getHostname() << std::endl;
            }
            else
            {
                // Dados recebidos de um cliente com ligacao ja estabelecida previamente
                clientSocket = irc.pollfds[i].fd;
                bytesRead = get_next_line(clientSocket, &message, 0);
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
                    Message	new_message(message, clientSocket);
                    broadcast(irc, &new_message, clientSocket);
                    logConsole(std::string(message));
                }
            }
        }
    }
}

int	main(int argc, char *argv[])
{
    if (argc != 3)
	{
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
            evaluatePing(irc);
        }
        closeFDs(irc);
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    return 0;
}
