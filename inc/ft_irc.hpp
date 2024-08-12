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
# include <climits>
# include "./gnl/get_next_line_bonus.h"
# include "Client.hpp"
# include "Message.hpp"
# include "Server.hpp"
# include "Channel.hpp"

// Error messages
# define ERRAM	"Error: activation of channel mode"
# define ERRC	"Error: obtaining the channel info"
# define ERRDM	"Error: deactivation of channel mode"
# define ERRJ	"Error: joining channel"
# define ERRM	"Error: obtaining the channel's modes list"
# define ERRP	"Error: parting channel"
# define ERRPM	"Error: sending private message"
# define ERRAOM	"Error: activation of channel operator mode"
# define ERRDOM	"Error: deactivation of channel operator mode"
# define ERRALM "Error: activation of channel's user limit"
# define ERRDLM "Error: deactivation of channel's user limit"
# define ERRLTH "Error: sending channel limit too high message"
# define ERRACK "Error: activation of channel's key"
# define ERRDCK "Error: deactivation of channel's key"
//# define ERR001 "Error: sending welcoming message (001)"
# define ERR353 "Error: sending channel's clients list and roles (353)"
# define ERR401 "Error: sending message of no such nick/channel (401)"
# define ERR403 "Error: sending message of no such channel (403)"
# define ERR405 "Error: sending message of too many channels (405)"
# define ERR412 "Error: sending message of no text to send (412)"
# define ERR417	"Error: sending message of input too long (417)"
# define ERR421 "Error: sending message of unknown command (421)"
# define ERR442	"Error: sending message of not on channel (442)"
# define ERR461	"Error: sending message of more parameters needed (461)"
# define ERR464 "Error: sending message of incorrect password (464)"
# define ERR471 "Error: sending message informing channel is full (471)"
# define ERR475 "Error: sending message of no channel permissions (475)"
# define ERR482 "Error: sending message of no operator permissions (482)"

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
# define ACTMODE(nick, chn, mode) (":" + nick + " MODE " + chn + " +" + mode)

// Deactivate Channel Mode
# define DEACTMODE(nick, chn, mode) (":" + nick + " MODE " + chn + " -" + mode)

// Activate Channel OperaCannot join channel (+k)tor Mode
# define RPL_CHANNELMODEISACT(nick, chn, mode, param) (":" + nick + " MODE " + chn + " +" + mode + " " + param)

// Dectivate Channel Operator Mode
# define RPL_CHANNELMODEISDEACT(nick, chn, mode, param) (":" + nick + " MODE " + chn + " -" + mode + " " + param)

// Sends a private message whether to a channel or another client
# define PRIVMSG(nick, nicks, msg) (":" + nick + " PRIVMSG " + nicks + " :" + msg)

// Channel limit too high
# define RPL_TOOHIGHLIMIT(hostname, nick, chn) \
	(":" + hostname + " 479 " + nick + " " + chn + " :Channel limit is too high")

// 324: Sent to a client to inform them of the currently-set modes of a channel
# define RPL_CHANNELMODEIS(hostname, nick, chn, modes) \
	(":" + hostname + " 324 " + nick + " " + chn + " :" + modes)

// 329: Sent to a client to inform them of the creation time of a channel
# define RPL_CREATIONTIME(hostname, nick, chn, time) \
	(":" + hostname + " 329 " + nick + " " + chn + " :" + time)

// 353: Reply to the NAMES command; lists the clients joined to a channel and their status
# define RPL_NAMREPLY(hostname, nick, chn, clients) \
	(":" + hostname + " 353 " + nick + " = " + chn + " :" + clients)

// 401: ndicates that no client can be found for the supplied nickname
# define ERR_NOSUCHNICK(hostname, nick) (hostname + " " + nick + " :No such nick/channel")

// 403: Indicates that no channel can be found for the supplied channel name
# define ERR_NOSUCHCHANNEL(hostname, chn) (hostname + " " + chn + " :No such channel")

// 405: Indicates that the JOIN command failed because the client has joined their maximum number of channels (10)
# define ERR_TOOMANYCHANNELS(hostname, chn) (hostname + " " + chn + " :You have joined too many channels")

// 412: Returned by the PRIVMSG command to indicate the message wasn’t delivered because there was no text to send
# define ERR_NOTEXTTOSEND(hostname) (hostname + " :No text to send")

// 417: Indicates a given line does not follow the specified size limits
# define ERR_INPUTTOOLONG(hostname) (hostname + " :Input line was too long")

// 421: Sent to a registered client to indicate that the command they sent isn’t known by the server
# define ERR_UNKNOWNCOMMAND(hostname, cmd) (hostname + " " + cmd + " :Unknown command")

// 442: Returned when a client tries to perform a channel-affecting command on a channel which the client isn’t a part of
# define ERR_NOTONCHANNEL(hostname, chn) (hostname + " " + chn + " :You're not on that channel")

// 461: Returned when a client command cannot be parsed because not enough parameters were supplied
# define ERR_NEEDMOREPARAMS(hostname, cmd) (hostname + " " + cmd + " :Not enough parameters")

// 464: Returned to indicate that the connection could not be registered as the password was either incorrect or not supplied
# define ERR_PASSWDMISMATCH(hostname) (hostname + " :Password incorrect")

// 471: indicates JOIN command failed because l mode is set and the max number of users was reached
# define ERR_CHANNELISFULL(hostname, nick, chn) \
	(":" + hostname + " 471 " + nick + " " + chn + " :Cannot join channel (+l)")

// 475: Returned to indicate that a JOIN command failed because the channel requires
// a key and the key was either incorrect or not supplied
# define ERR_BADCHANNELKEY(nick, chn) (nick + " " + chn + " :Cannot join channel (+k)")

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
void		cmdPing(Server &irc, Message *message, int sender);
void 		cmdPong(Server &irc, Message *message, int sender);

// Utilities
void 		closeFDs(Server &irc);
void 		sigHandler(int signal);
void 		setNonBlocking(int socket);
void    	evaluatePing(Server &irc);
std::string	toUpper(const std::string& str);
int			aux(std::string targets);

// Send
void    	sendSequenceRPL(Server &irc, Message *message, int sender);
void    	sendMOTD(Server &irc, Message *message, int sender);
void 		logConsole(std::string buffer);
void		sendMessage(int fd, const std::string& msg, const std::string& emsg);
void		sendMessageAll(int fd, std::vector<int> fds, const std::string& msg, const std::string& emsg);
std::vector<std::string> split(const std::string &s, char delim);
// Other
std::string	cleanString(const std::string& name);

#endif
