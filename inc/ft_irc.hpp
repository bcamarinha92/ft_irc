#ifndef FT_IRC_HPP
#define FT_IRC_HPP

#pragma once

#include <iostream>
#include <cstring>
#include <vector>
#include <map>
#include <string>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <poll.h>
#include <fcntl.h>
#include <cstdlib>
#include <cstdio>
#include "Client.hpp"
#include "Message.hpp"
#include "Server.hpp"
#include <exception>
#include <csignal>
#include <sstream>
#include "./gnl/get_next_line_bonus.h"

#ifndef BUFFER_SIZE
#define BUFFER_SIZE 10
#endif
#ifndef MAX_FD
#define MAX_FD 2048
#endif

void setNonBlocking(int socket);
std::string getNickFromBuffer(const std::string &input);
std::string getChannelFromBuffer(const std::string &input);
std::string get_buffer_command(const std::string buffer);
std::vector<std::string> get_buffer_parameters(const std::string &buffer);

#endif