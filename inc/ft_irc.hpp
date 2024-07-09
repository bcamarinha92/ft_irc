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
# include "./gnl/get_next_line_bonus.h"

#define MAX_CLIENTS 100

static bool running;

void setNonBlocking(int socket);

#endif