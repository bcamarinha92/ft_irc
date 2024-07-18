#ifndef FT_IRC_HPP
# define FT_IRC_HPP

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
# include <netdb.h>
# include <exception>
# include <csignal>
# include <sstream>
# include "Client.hpp"
# include "Server.hpp"
# include <cstdio>
# include "Channel.hpp"
# include <utility>
# include <algorithm>
# include "./gnl/get_next_line_bonus.h"

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 10
# endif
# ifndef MAX_FD
#  define MAX_FD 2048
# endif

// exemplos de defines para as mensagens enviadas para o cliete 

# define JOIN(nick, channel) \
    (":" + nick + " JOIN " + channel + "\r\n")

# define RLP_WELCOME() \
    ("localhost 001 bde-sous :Welcome to the Internet Relay Network \r\n")
 
static bool running;

void setNonBlocking(int socket);
std::string getNickFromBuffer(const std::string& input);
std::string getChannelFromBuffer(const std::string& input);

#endif
