#include "../inc/ft_irc.hpp"



void setNonBlocking(int socket) {
    int flags = fcntl(socket, F_GETFL, 0);
    fcntl(socket, F_SETFL, flags | O_NONBLOCK);
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " <port> <password>" << std::endl;
        return 1;
    }

    int port = atoi(argv[1]);
    std::string password = argv[2];

    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket < 0) {
        perror("socket");
        return 1;
    }

    sockaddr_in serverAddr;
    std::memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(port);

    if (bind(serverSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
        perror("bind");
        close(serverSocket);
        return 1;
    }

    if (listen(serverSocket, MAX_CLIENTS) < 0) {
        perror("listen");
        close(serverSocket);
        return 1;
    }

    setNonBlocking(serverSocket);

    std::vector<pollfd> pollfds;
    std::map<int, Client> clients;

    pollfd serverPollfd;
    serverPollfd.fd = serverSocket;
    serverPollfd.events = POLLIN;
    pollfds.push_back(serverPollfd);

    while (true) {
        int pollCount = poll(pollfds.data(), pollfds.size(), -1);
        if (pollCount < 0) {
            perror("poll");
            break;
        }

        for (size_t i = 0; i < pollfds.size(); ++i) {
            if (pollfds[i].revents & POLLIN) {
                if (pollfds[i].fd == serverSocket) {
                    // Nova conexão
                    sockaddr_in clientAddr;
                    socklen_t clientAddrLen = sizeof(clientAddr);
                    int clientSocket = accept(serverSocket, (sockaddr*)&clientAddr, &clientAddrLen);
                    if (clientSocket < 0) {
                        perror("accept");
                        continue;
                    }
                    setNonBlocking(clientSocket);

                    pollfd clientPollfd;
                    clientPollfd.fd = clientSocket;
                    clientPollfd.events = POLLIN;
                    pollfds.push_back(clientPollfd);

                    clients[clientSocket] = Client(clientSocket);
                    std::cout << "New connection from " << inet_ntoa(clientAddr.sin_addr) << std::endl;
                } else {
                    // Dados recebidos de um cliente
                    int clientSocket = pollfds[i].fd;
                    char buffer[BUFFER_SIZE];
                    int bytesRead = read(clientSocket, buffer, sizeof(buffer));
                    std::cout << bytesRead <<std::endl;
                    if (bytesRead <= 0) {
                        if (bytesRead < 0) perror("read");
                        close(clientSocket);
                        pollfds.erase(pollfds.begin() + i);
                        clients.erase(clientSocket);
                        --i;
                        std::cout << "Client disconnected" << std::endl;
                    } else {
                        buffer[bytesRead] = '\0';
						Message new_message(buffer);
						std::cout << "Received message: " << new_message << std::endl;
                    }
                }
            }
        }
    }
    close(serverSocket);
    return 0;
}
