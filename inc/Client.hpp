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
		std::string		getRealname() const;
		std::string		getHostname() const;
		std::string 	getServername() const;
		sockaddr_in		getclientAddr() const;
		socklen_t		getclientAddrLen() const;
		void 			setSocket(int socket);
		void 			setNickname(const std::string& nickname);
		void 			setUsername(const std::string& username);
		void			setRealname(const std::string& realname);
		void			setHostname(const std::string& hostname);
		void			setServername(const std::string& servername);
		pollfd 			clientPollfd;
	private:
		std::string		_nickname;
    	std::string 	_username;
		std::string		_realname;
		std::string		_hostname;
		std::string		_servername;
		sockaddr_in		_clientAddr;
		socklen_t 		_clientAddrLen;
		int 			_clientSocket;
		char 			*_client_ip;		
};

std::ostream &			operator<<( std::ostream & o, Client const & i );

#endif /* ********************************************************** CLIENT_H */
