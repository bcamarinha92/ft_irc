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
# include <cstdio> 
# include "Client.hpp"
# include "Server.hpp"
# include <exception>
# include <csignal>
# include <sstream>

#define MAX_CLIENTS 100
#define BUFFER_SIZE 512

static bool running;

void setNonBlocking(int socket);

#endif