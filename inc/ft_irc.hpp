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
# include <netdb.h>
# include <exception>
# include <csignal>
# include <sstream>
# include <cstdio>
# include <cstdio>
# include <utility>
# include <algorithm>
# include <ctime>
# include <climits>
# include <ctime>
# include <climits>
# include "./gnl/get_next_line_bonus.h"
# include "Client.hpp"
# include "Message.hpp"
# include "Server.hpp"
# include "Channel.hpp"

class Server;
class Message;

// ---------------------------------------------- MACROS ----------------------------------------------
//
# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 10
# endif
# ifndef MAX_FD
#  define MAX_FD 2048
# endif
# ifndef CHANLIMIT
#  define CHANLIMIT 10
# endif

// ------------------------------------------ ERROR MESSAGES ------------------------------------------
//
# define ERRACK "Error: activation of channel's key"
# define ERRALM "Error: activation of channel's user limit"
# define ERRAM	"Error: activation of channel mode"
# define ERRAOM	"Error: activation of channel operator mode"
# define ERRC	"Error: obtaining the channel info"
# define ERRCAP	"Error: sending CAP message"
# define ERRDCK "Error: deactivation of channel's key"
# define ERRDLM "Error: deactivation of channel's user limit"
# define ERRDM	"Error: deactivation of channel mode"
# define ERRDOM	"Error: deactivation of channel operator mode"
# define ERRJ	"Error: joining channel"
# define ERRK	"Error: kicking someone from a channel"
# define ERRLTH "Error: sending channel limit too high message"
# define ERRM	"Error: obtaining the channel's modes list"
# define ERRNOT	"Error: sending Notice message"
# define ERRNTM	"Error: sending no +t mode activated message"
# define ERRP	"Error: parting channel"
# define ERRPIN	"Error: sending PING message"
# define ERRPM	"Error: sending private message"

# define ERR001 "Error: sending welcoming message (001)"
# define ERR002 "Error: sending post-registration greeting (002)"
# define ERR003 "Error: sending post-registration greeting (003)"
# define ERR004 "Error: sending post-registration greeting (004)"
# define ERR005 "Error: sending RPL_ISUPPORT after client registration (005)"
# define ERR315 "Error: sending end of WHO message (315)"
# define ERR331	"Error: sending no topic is set message (331)"
# define ERR332	"Error: sending topic message (332)"
# define ERR341 "Error: sending inviting message (341)"
# define ERR352	"Error: sending a reply to the WHO command (352)"
# define ERR353 "Error: sending channel's clients list and roles (353)"
# define ERR366	"Error: sending end of names list message (366)"
# define ERR401 "Error: sending message of no such nick/channel (401)"
# define ERR403 "Error: sending message of no such channel (403)"
# define ERR404 "Error: Cannot send to channel (404)"
# define ERR405 "Error: sending message of too many channels (405)"
# define ERR412 "Error: sending message of no text to send (412)"
# define ERR417	"Error: sending message of input too long (417)"
# define ERR421 "Error: sending message of unknown command (421)"
# define ERR422 "Error: sending mestd::age of no MOTD found (422)"
# define ERR431	"Error: sending no nickname given message (431)"
# define ERR433	"Error: sending nickname already set message (433)"
# define ERR441	"Error: sending user not in channel message (441)"
# define ERR442	"Error: sending message of not on channel (442)"
# define ERR443 "Error: sending message of user already on channel (443)"
# define ERR461	"Error: sending message of more parameters needed (461)"
# define ERR462	"Error: sending already registered message (462)"
# define ERR464 "Error: sending message of incorrect password (464)"
# define ERR471 "Error: sending message informing channel is full (471)"
# define ERR473	"Error: sending message informing the channel is invite only (473)"
# define ERR475 "Error: sending message of no channel permissions (475)"
# define ERR482 "Error: sending message of no operator permissions (482)"

// ----------------------------------------- SERVER MESSAGES -----------------------------------------
//
// /////// COMMANDS ///////
//
// CAP
# define CAP(nick) (":" + nick + " CAP * LS :")

// JOIN
# define JOIN(nick, chn) (":" + nick + " JOIN " + chn)

// KICK
# define KICK(hostname, chn, user, reason) \
	(":" + hostname + " KICK " + chn + " " + user + " :" + reason)

// PING
# define PING(hostname, buffer) (":" + hostname + " PING " + buffer)

// MODE
	// Activate Channel Mode
	# define ACTMODE(nick, chn, mode) (":" + nick + " MODE " + chn + " +" + mode)

	// Deactivate Channel Mode
	# define DEACTMODE(nick, chn, mode) (":" + nick + " MODE " + chn + " -" + mode)

	// Activate Channel Operator Mode
	# define RPL_CHANNELMODEISACT(nick, chn, mode, param) \
		(":" + nick + " MODE " + chn + " +" + mode + " " + param)

	// Dectivate Channel Operator Mode
	# define RPL_CHANNELMODEISDEACT(nick, chn, mode, param) \
		(":" + nick + " MODE " + chn + " -" + mode + " " + param)

// NOTICE
# define NOTICE(nick, hostname, dest, msg) (":" + nick + "!" + nick + "@" + hostname + " NOTICE " + dest + " :" + msg)

// PRIVMSG
# define PRIVMSG(nick, dest, msg) (":" + nick + " PRIVMSG " + dest + " :" + msg)

// /////// RPL & ERR MESSAGES ///////
//
// Channel limit too high
# define RPL_TOOHIGHLIMIT(hostname, nick, chn) \
	(":" + hostname + " 479 " + nick + " " + chn + " :Channel limit is too high")

// Channel does not have +t mode
# define ERR_NOTMODE(hostname, nick, chn) \
	(":" + hostname + " 482 " + nick + " " + chn + " :You have been invited to")

// Notification of new invite
# define RPL_RECINVITE(host)

// 001: The first message sent after client registration, this message introduces the client to the network
# define RPL_WELCOME(hostserver, nick) \
	(":" + hostserver + " 001 " + nick + " :Welcome to the Paulo Brificado's IRC " + nick + "!")

// 002: Part of the post-registration greeting, this numeric returns the name and software/version of the
// server the client is currently connected to
# define RPL_YOURHOST(hostserver, nick, hostclient) \
	(":" + hostserver + " 002 " + nick + " :Your host is " + hostclient + ", running version 0.01")

// 003: Part of the post-registration greeting, this numeric returns a human-readable date/time
// that the server was started or created
# define RPL_CREATED(hostserver, nick, time) \
	(":" + hostserver + " 003 " + nick + " :This server was created at " + time)

// 004: Part of the post-registration greeting. Clients SHOULD discover available features using
// RPL_ISUPPORT tokens rather than the mode letters listed in this reply
# define RPL_MYINFO(hostserver, nick) \
	(":" + hostserver + " 004 " + nick + " :" + hostserver + " 0.01 ao itkol")

// 005: A server MUST issue at least one RPL_ISUPPORT numeric after client registration has completed.
// It MUST be issued before further commands from the client are processed
# define RPL_ISUPPORT(hostserver, nick) \
	(":" + hostserver + " 005 " + nick + " :PREFIX=(o)@ :CHANTYPES=# :CHANMODES=i,t,k,o,l :CASEMAPPING=ascii :TARGMAX=PART:1,NAMES:1,KICK:1,INVITE1:,WHO:1,WHOIS:1,PRIVMSG:1 :NETWORK=PauloBrificado :are supported by this server")

// 315: Sent as a reply to the WHO command, this numeric indicates the end of a WHO response for
// the mask <mask>
# define RPL_ENDOFWHO(hostname, nick, dest) \
	(":" + hostname + " 315 " + nick + " " + dest + " :End of /WHO list")

// 324: Sent to a client to inform them of the currently-set modes of a channel
# define RPL_CHANNELMODEIS(hostname, nick, chn, modes) \
	(":" + hostname + " 324 " + nick + " " + chn + " :" + modes)

// 329: Sent to a client to inform them of the creation time of a channel
# define RPL_CREATIONTIME(hostname, nick, chn, time) \
	(":" + hostname + " 329 " + nick + " " + chn + " :" + time)

// 331: Sent to a client when joining a channel to inform them that the channel with the name <channel>
// does not have any topic set
# define RPL_NOTOPIC(hostname, nick, chn) \
	(":" + hostname + " 331 " + nick + " " + chn + " :No topic is set")

// 332: Sent to a client when joining the <channel> to inform them of the current topic of the channel.
# define RPL_TOPIC(hostname, nick, chn, topic) \
	(":" + hostname + " 332 " + nick + " " + chn + " :" + topic)

// 341: Sent as a reply to the INVITE command to indicate that the attempt was successful and
// the client with the nickname <nick> has been invited to <channel>.
# define RPL_INVITING(hostname, nicko, chn, nick) \
	(":" + hostname + " 341 " + nicko + " " + nick + " " + chn)

// 352: Sent as a reply to the WHO command, this numeric gives information about the client with
// the nickname <nick>
# define RPL_WHOREPLY(hostname, nick, dest, user, chost, realname) \
	(":" + hostname + " 352 " + nick + " " + dest  + " " + user + " " + chost + " " + hostname + " " + nick + " H@ :0 " + realname + "")

// 353: Reply to the NAMES command; lists the clients joined to a channel and their status
# define RPL_NAMREPLY(hostname, nick, chn, clients) \
	(":" + hostname + " 353 " + nick + " = " + chn + " :" + clients)

// 366: Sent as a reply to the NAMES command, this numeric specifies the end of a list of channel
// member names.
# define RPL_ENDOFNAMES(hostname, nick, chn) \
	(":" + hostname + " 366 " + nick + " " + chn + " :End of /NAMES list")

// 401: ndicates that no client can be found for the supplied nickname
# define ERR_NOSUCHNICK(hostname, nick) (hostname + " " + nick + " :No such nick/channel")

// 403: Indicates that no channel can be found for the supplied channel name
# define ERR_NOSUCHCHANNEL(hostname, chn) (hostname + " " + chn + " :No such channel")

// 404: Indicates that the PRIVMSG / NOTICE could not be delivered to <channel>
# define ERR_CANNOTSENDTOCHAN(hostname, chn) (hostname + " " + chn + " :Cannot send to channel")

// 405: Indicates that the JOIN command failed because the client has joined their maximum number
// of channels
# define ERR_TOOMANYCHANNELS(hostname, chn) (hostname + " " + chn + " :You have joined too many channels")

// 412: Returned by the PRIVMSG command to indicate the message wasn’t delivered because there was
// no text to send
# define ERR_NOTEXTTOSEND(hostname) (hostname + " :No text to send")

// 417: Indicates a given line does not follow the specified size limits
# define ERR_INPUTTOOLONG(hostname) (hostname + " :Input line was too long")

// 421: Sent to a registered client to indicate that the command they sent isn’t known by the server
# define ERR_UNKNOWNCOMMAND(hostname, cmd) (hostname + " " + cmd + " :Unknown command")

// 422: Indicates that the Message of the Day file does not exist or could not be found
# define ERR_NOMOTD(hostname, nick) (":" + hostname + " 422 " + nick + " :MOTD File is missing")

// 431: Returned when a nickname parameter is expected for a command but isn’t given
# define ERR_NONICKNAMEGIVEN(hostname) (":" + hostname + " 431 * :No nickname given")

// 433: Returned when a NICK command cannot be successfully completed as the desired nickname is
// already in use on the network
# define ERR_NICKNAMEINUSE(hostname, nick) \
	(":" + hostname + " 433 " + nick + " :Nickname is already set")

// 441: Returned when a client tries to perform a channel+nick affecting command, when the nick
// isn’t joined to the channel
# define ERR_USERNOTINCHANNEL(hostname, nick, param, chn) \
	(":" + hostname + " 441 " + nick + " " + param + " " + chn + " :You're not on that channel")

// 442: Returned when a client tries to perform a channel-affecting command on a channel which the
// client isn’t a part of
# define ERR_NOTONCHANNEL(hostname, chn) (hostname + " " + chn + " :You're not on that channel")

// 443: Returned when a client tries to invite <nick> to a channel they’re already joined to.
# define ERR_USERONCHANNEL(hostname, nick, chn) \
		(hostname + " " + nick + " " + chn + " :is already on channel")

// 461: Returned when a client command cannot be parsed because not enough parameters were supplied
# define ERR_NEEDMOREPARAMS(hostname, cmd) (hostname + " " + cmd + " :Not enough parameters")

// 462: Error message when already registered
# define ERR_ALREADYREGISTERED(hostname, nick) \
		(":" + hostname + " 462 " + nick + " :You may not reregister")

// 464: Returned to indicate that the connection could not be registered as the password was either
// incorrect or not supplied
# define ERR_PASSWDMISMATCH(hostname) (hostname + " :Password incorrect")

// 471: indicates JOIN command failed because l mode is set and the max number of users was reached
# define ERR_CHANNELISFULL(hostname, nick, chn) \
	(":" + hostname + " 471 " + nick + " " + chn + " :Cannot join channel (+l)")

// 472: Returned to indicate that a JOIN command failed because the channel is set to [invite-only] mode
// and the client has not been invited to the channel
# define ERR_INVITEONLYCHAN(nick, chn) (nick + " " + chn + " :Cannot join channel (+i)")

// 475: Returned to indicate that a JOIN command failed because the channel requires
// a key and the key was either incorrect or not supplied
# define ERR_BADCHANNELKEY(nick, chn) (nick + " " + chn + " :Cannot join channel (+k)")

// 482: Error message of trying to change a channel mode without the operator role
# define ERR_CHANOPRIVSNEEDED(hostname, nick, chn) \
	(":" + hostname + " 482 " + nick + " " + chn + " :You're not channel operator")

// --------------------------------------------- GLOBALS ---------------------------------------------
//
static bool running;
static char	*pos[MAX_FD];
static bool	isbot;

// -------------------------------------------- FUNCTIONS --------------------------------------------
//
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
void		cmdUser(Server &irc, Message *message, int sender);
void        cmdTopic(Server &irc, Message *message, int sender);
void        cmdKick(Server &irc, Message *message, int sender);
void        nameReply(Server &irc, std::string chn, int sender);
void		cmdInvite(Server &irc, Message *message, int sender);
void	    cmdNotice(Server &irc, Message *message, int sender);

// Parser
std::string 				getNickFromBuffer(const std::string& input);
std::string 				getChannelFromBuffer(const std::string& input);
std::string					getModeFromBuffer(const std::string& input);
std::vector<std::string>    get_buffer_parameters(const std::string &buffer);
std::string                 get_buffer_command(const std::string buffer);
std::string 				parseRealname(const std::string& input);

// Send
void	sendSequenceRPL(Server &irc, Message *message, int sender);
void 	logConsole(std::string buffer);
void	sendMessage(int fd, const std::string& msg, const std::string& emsg);
void	sendMessageAll(int fd, std::vector<int> fds, const std::string& msg, const std::string& emsg, bool pm);
void	sendPrivMsg(int fd, std::vector<int> fds, const std::string& msg, const std::string& emsg);
void	sendM(int fd, const std::string& msg, const std::string& emsg);

// Utilities
void 						closeFDs(Server &irc);
void 						sigHandler(int signal);
void 						setNonBlocking(int socket);
void    					evaluatePing(Server &irc);
std::string					toUpper(const std::string& str);
std::vector<std::string>	split(const std::string &s, char delim);
std::string					cleanString(const std::string& name);
