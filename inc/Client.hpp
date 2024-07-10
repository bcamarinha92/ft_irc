#ifndef CLIENT_HPP
# define CLIENT_HPP

# include "ft_irc.hpp"

class Client
{

	public:

		Client();
		Client(int socket);
		Client(Client const& src);
		~Client();
		Client&			operator=(Client const& rhs);
		int 			getSocket() const;
		std::string 	getNickname() const;
		std::string 	getUsername() const;
		sockaddr_in		getclientAddr() const;
		socklen_t		getclientAddrLen() const;
		void 			setSocket(int socket);
		void 			setNickname(const std::string& nickname);
		void 			setUsername(const std::string& username);
		pollfd 			clientPollfd;

	private:
		std::string		_nickname;
    	std::string 	_username;
		sockaddr_in		_clientAddr;
		socklen_t 		_clientAddrLen;
		int 			_clientSocket;
};

std::ostream &			operator<<( std::ostream & o, Client const & i );

#endif /* ********************************************************** CLIENT_H */
