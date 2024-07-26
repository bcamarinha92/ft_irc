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
# define ERR7 "Error: sending message of no operator permissions (482)"
# define ERR8 "Error: sending channel's clients list and roles (353)"
# define ERR9 "Error: sending welcoming message (001)"
# define ERR10 "Error: sending private message"
# define ERR11 "Error: activation of channel operator mode"
# define ERR12 "Error: deactivation of channel operator mode"
# define ERR13 "Error: sending message informing channel is full"
# define ERR14 "Error: activation of channel's user limit"
# define ERR15 "Error: deactivation of channel's user limit"

class Server;
class Message;

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 10
# endif
# ifndef MAX_FD
#  define MAX_FD 2048
# endif

// ----------------------------------------- SERVER MESSAGES -----------------------------------------

// Join of a client to a channel
# define JOIN(nick, chn) (":" + nick + " JOIN " + chn)

// Activate Channel Mode
# define ACTMODE(nick, chn, mode) (":"+ nick + " MODE " + chn + " +" + mode)

// Deactivate Channel Mode
# define DEACTMODE(nick, chn, mode) (":"+ nick + " MODE " + chn + " -" + mode)

// Activate Channel Operator Mode
# define RPL_CHANNELMODEISACT(nick, chn, mode, param) (":"+ nick + " MODE " + chn + " +" + mode + " " + param)

// Dectivate Channel Operator Mode
# define RPL_CHANNELMODEISDEACT(nick, chn, mode, param) (":"+ nick + " MODE " + chn + " -" + mode + " " + param)

// Sends a private message whether to a channel or another client
# define PRIVMSG(nick, msg) (":" + nick + " " + msg)

// 324: Sent to a client to inform them of the currently-set modes of a channel
# define RPL_CHANNELMODEIS(hostname, nick, chn, modes) \
	(":" + hostname + " 324 " + nick + " " + chn + " :" + modes)

// 329: Sent to a client to inform them of the creation time of a channel
# define RPL_CREATIONTIME(hostname, nick, chn, time) \
	(":" + hostname + " 329 " + nick + " " + chn + " :" + time)

// 353: Reply to the NAMES command; lists the clients joined to a channel and their status
# define RPL_NAMREPLY(hostname, nick, chn, clients) \
	(":" + hostname + " 353 " + nick + " = " + chn + " :" + clients)

// 471: indicates JOIN command failed because l mode is set and the max number of users was reached
# define ERR_CHANNELISFULL(hostname, nick, chn) \
	(":" + hostname + " 471 " + nick + " " + chn + " :Cannot join channel (+l)")

// 482: Error message of trying to change a channel mode without the operator role
# define ERR_CHANOPRIVSNEEDED(hostname, nick, chn) \
	(":" + hostname + " 482 " + nick + " " + chn + " :You're not channel operator")

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
void    	cmdPing(Message *message, int sender);

// Utilities
void 		closeFDs(Server &irc);
void 		sigHandler(int signal);
void 		setNonBlocking(int socket);
void 		logConsole(std::string buffer);
void		sendMessage(int fd, std::vector<int> fds, const std::string& msg, const std::string& emsg, bool all);
void    	sendSequenceRPL(Server &irc, Message *message, int sender);
void    	sendMOTD(Server &irc, Message *message, int sender);
std::string	getCurrentDateTime();

// Other
std::string	cleanString(const std::string& name);

#endif
