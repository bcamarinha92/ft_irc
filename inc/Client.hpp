#ifndef CLIENT_HPP
# define CLIENT_HPP

# include "ft_irc.hpp"

class Channel;

class Client
{

	public:
		//Constructors
		Client();
		Client(int socket);
		Client(Client const& src);

		//Destructors
		~Client();

		//Overloads
		Client&							operator=(Client const& rhs);

		//Getters
		int 							getSocket() const;
		std::string 					getNickname() const;
		std::string 					getUsername() const;
		sockaddr_in						getclientAddr() const;
		socklen_t						getclientAddrLen() const;

		//Setters
		void 							setSocket(int socket);
		void 							setNickname(const std::string& nickname);
		void 							setUsername(const std::string& username);

		//Methods
		void							addChannel(const Channel& channel);
		void							rmChannel(std::string channelName);

		//Attributes
		pollfd 							clientPollfd;
		std::map<std::string, Channel>	channels;

	private:
		//Attributes
		std::string						_nickname;
    	std::string 					_username;
		sockaddr_in						_clientAddr;
		socklen_t	 					_clientAddrLen;
		int				 				_clientSocket;
		char 							*_client_ip;
};

std::ostream&			operator<<(std::ostream& o, Client const& i);

#endif /* ********************************************************** CLIENT_H */
