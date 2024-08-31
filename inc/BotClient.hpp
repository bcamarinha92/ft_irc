#ifndef BOTCLIENT_HPP
# define BOTCLIENT_HPP

# include "ft_irc.hpp"

class BotClient
{
	public:
		//Constructors
		BotClient(Server& irc);
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

		//Methods

		//Attributes

	private:
		//Attributes
		int					_socket;
		std::string			_ipServer;
		int					_port;
		std::string			_password;
		std::string			_nickname1;
		std::string			_nickname2;
		std::string			_nickname3;
    	std::string			_username;
		std::string			_realname;
		struct sockaddr_in	_server;
};

#endif
