#ifndef FT_IRC_HPP
#define FT_IRC_HPP

# pragma once

# include <iostream>
# include <string>
# include <cctype>
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
# include <cstdio>
# include <utility>
# include <algorithm>
# include <ctime>
# include "./gnl/get_next_line_bonus.h"
# include "Client.hpp"
# include "Message.hpp"
# include "Server.hpp"
# include "Channel.hpp"

// Error messages
# define ERR1 "Error: joining channel"
# define ERR2 "Error: obtaining the channel's modes list"
# define ERR3 "Error: obtaining the channel info"
# define ERR4 "Error: activation of channel mode"
# define ERR5 "Error: deactivation of channel mode"
# define ERR6 "Error: parting channel"

class Server;
class Message;

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

// Get Strings from Buffer
std::string 				getNickFromBuffer(const std::string& input);
std::string 				getChannelFromBuffer(const std::string& input);
std::string					getModeFromBuffer(const std::string& input);
std::vector<std::string>    get_buffer_parameters(const std::string &buffer);
std::string                 get_buffer_command(const std::string buffer);

// Commands
void		cmdNick(Server &irc, Message *message, int sender);
void		cmdJoin(Server &irc, Message *message, int sender);
void		cmdWho(Server &irc, Message *message, int sender);
void		cmdPass(Server &irc, Message *message, int sender);
void		cmdPrivMsg(Server &irc, Message *message, int sender);
void		cmdMode(Server &irc, Message *message, int sender);
void		cmdCap(Server &irc, Message *message, int sender);
void		cmdPart(Server &irc, Message *message, int sender);

// Utilities
void 		closeFDs(Server &irc);
void 		sigHandler(int signal);
void 		setNonBlocking(int socket);
void 		logConsole(std::string buffer);
void		sendMessage(int fd, std::vector<int> fds, const std::string& msg, const std::string& emsg, bool all);

// Other
std::string	cleanString(const std::string& name);

#endif
