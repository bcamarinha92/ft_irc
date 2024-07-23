#ifndef FT_IRC_HPP
#define FT_IRC_HPP

# pragma once

# include <iostream>
# include <string>
# include <cctype>
# include <ctime>
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
# include "Message.hpp"
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
class Server;
class Message;

void                        setNonBlocking(int socket);
std::string                 getNickFromBuffer(const std::string& input);
std::string                 getChannelFromBuffer(const std::string& input);
void                        cmdNick(Server &irc, Message *message, int sender);
void                        cmdJoin(Server &irc, Message *message, int sender);
void                        cmdWho(Server &irc, std::string chn, int sender);
void                        cmdPass(Server &irc, Message *message, int sender);
void                        cmdPrivMsg(Server &irc, Message *message, int sender);
void                        cmdCap(Server &irc, Message *message, int sender);
void	                    who(int sender, Server &irc, std::string const& chn, bool op);
std::vector<std::string>    get_buffer_parameters(const std::string &buffer);
std::string                 get_buffer_command(const std::string buffer);
void                        logConsole(std::string buffer);

#endif
