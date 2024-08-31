#ifndef BOTCLIENT_HPP
# define BOTCLIENT_HPP

# include <iostream>
# include <string>
# include <sys/socket.h>
# include <netinet/in.h>
# include <arpa/inet.h>
# include <vector>
# include <sstream>
# include <cstdlib>
# include <ctime>
# include <cstring>

class BotClient
{
	public:
		//Constructors
		BotClient(std::string serverIP, int port, std::string pass);
		BotClient(const BotClient& src);

		//Overloads
		BotClient	&operator=(const BotClient& rhs);

		//Destructors
		~BotClient();

		//Getters
		int					getSocket() const;
		int 				getPingCount() const;
		std::string			getNickname() const;
		std::string 		getRealname() const;
		std::string			getUsername() const;
		std::string			getPassword() const;

		//Attributes
		std::vector<std::string>	msgs;

	private:
		//Attributes
		int							_socket;
		std::string					_ipServer;
		int							_port;
		std::string					_password;
		std::string					_nickname1;
		std::string					_nickname2;
		std::string					_nickname3;
    	std::string					_username;
		std::string					_realname;
		struct sockaddr_in			_server;
};

std::vector<std::string>	split(const std::string &s, char delim);
int 						generateRandomNumber();
void						cmdPing(std::vector<std::string> elems, size_t i, BotClient& b);
void						cmdPrivMsg(std::vector<std::string> elems, size_t i, std::string buffer, BotClient& b);
void						cmdInvite(std::vector<std::string> elems, size_t i, BotClient& b);

#endif
