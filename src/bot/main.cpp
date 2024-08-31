#include <iostream>
#include <string>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define IRC_SERVER "127.0.0.1"
#define IRC_PORT 6697
#define IRC_CHANNEL "#test"
#define IRC_NICK "mybot"
#define PASS "123"

int main() {
    int sock;
    struct sockaddr_in server;
    char buffer[256];

    // Create socket
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        std::cerr << "Error creating socket" << std::endl;
        return 1;
    }

    // Set up server address
    server.sin_family = AF_INET;
    server.sin_port = htons(IRC_PORT);
    inet_pton(AF_INET, IRC_SERVER, &server.sin_addr);
    std::cout << "Connecting to " << IRC_SERVER << " on port " << IRC_PORT << std::endl;
    // Connect to server
    if (connect(sock, (struct sockaddr *)&server, sizeof(server)) < 0) {
        std::cerr << "Error connecting to server" << std::endl;
        return 1;
    }

    std::string passCmd = "PASS " + std::string(PASS) + "\r\n";
    send(sock, passCmd.c_str(), passCmd.length(), 0);
    // Send nick and user commands
    std::string nickCmd = "NICK " + std::string(IRC_NICK) + "\r\n";
    send(sock, nickCmd.c_str(), nickCmd.length(), 0);
    std::string userCmd = "USER " + std::string(IRC_NICK) + " " + std::string(IRC_NICK) + " bla :mybot\r\n";
    send(sock, userCmd.c_str(), userCmd.length(), 0);

    // Join channel
    std::string joinCmd = "JOIN " + std::string(IRC_CHANNEL) + "\r\n";
    send(sock, joinCmd.c_str(), joinCmd.length(), 0);

    // Send message
    std::string msgCmd = "PRIVMSG " + std::string(IRC_CHANNEL) + " :Hello, world!\r\n";
    send(sock, msgCmd.c_str(), msgCmd.length(), 0);

    // Receive and print messages
    while (true) {
        recv(sock, buffer, 256, 0);
        std::cout << buffer << std::endl;
    }

    return 0;
}
