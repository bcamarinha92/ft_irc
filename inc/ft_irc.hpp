# pragma once

# include <iostream>
# include <cstring>
# include <vector>
# include <map>
# include <string>
# include <netinet/in.h>
# include <arpa/inet.h>
# include <unistd.h>
# include <poll.h>
# include <fcntl.h>
# include <cstdlib>
# include <cstdio>
# include "Client.hpp"
# include "Channel.hpp"
# include "Server.hpp"
# include <exception>
# include <csignal>
# include <sstream>
# include <utility>
# include <algorithm>
# include "./gnl/get_next_line_bonus.h"

class Server;

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 10
# endif
# ifndef MAX_FD
#  define MAX_FD 2048
# endif

// exemplos de defines para as mensagens enviadas para o cliete
# define JOIN(nick, channel) \
    (":" + nick + " JOIN " + channel)

# define RLP_WELCOME() \
    ("localhost 001 bde-sous :Welcome to the Internet Relay Network \r\n")

static bool running;

// Get Strings from Buffer
std::string getNickFromBuffer(const std::string& input);
std::string getChannelFromBuffer(const std::string& input);
std::string	getModeFromBuffer(const std::string& input);

// Commands
void        cmdNick(Server &irc, char *buffer, int sender);
void        cmdJoin(Server &irc, char *buffer, int sender);
void        cmdWho(Server &irc, char *buffer, int sender);
void		cmdMode(Server &irc, char *buffer, int sender);

// Other
void 		setNonBlocking(int socket);
void 		logConsole(std::string buffer);
void        printClientMap(const std::map<int, Client*> clientMap);

