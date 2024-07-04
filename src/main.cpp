#include "../inc/ft_irc.hpp"

void setNonBlocking(int socket) 
{
    int flags = fcntl(socket, F_GETFL, 0);
    fcntl(socket, F_SETFL, flags | O_NONBLOCK);
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
        while (true) {
            int pollCount = poll(irc.pollfds.data(), irc.pollfds.size(), -1);
            if (pollCount < 0) {
                perror("poll");
                break;
            }

            for (size_t i = 0; i < irc.pollfds.size(); ++i) {
                if (irc.pollfds[i].revents & POLLIN) {
                    if (irc.pollfds[i].fd == irc.getServerSocket()) {
                        // Nova conexão detetada
                        Client user(irc.getServerSocket());
                        irc.pollfds.push_back(user.clientPollfd);
                        irc.clients[user.getSocket()] = user;
                        std::cout << "New connection from " << inet_ntoa(user.getclientAddr().sin_addr) << std::endl;
                    } else {
                        // Dados recebidos de um cliente com ligacao ja estabelecida previamente
                        int clientSocket = irc.pollfds[i].fd;
                        char buffer[BUFFER_SIZE];
                        int bytesRead = read(clientSocket, buffer, sizeof(buffer));
                        std::cout << bytesRead <<std::endl;
                        if (bytesRead <= 0) {
                            if (bytesRead < 0) perror("read");
                            close(clientSocket);
                            irc.pollfds.erase(irc.pollfds.begin() + i);
                            irc.clients.erase(clientSocket);
                            --i;
                            std::cout << "Client disconnected" << std::endl;
                        } else {
                            buffer[bytesRead] = '\0';
                            std::cout << "Received message: " << buffer << std::endl;
                        }
                    }
                }
            }
        }
        close(irc.getServerSocket());
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    return 0;
}
