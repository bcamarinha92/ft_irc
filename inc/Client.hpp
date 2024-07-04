#ifndef CLIENT_HPP
# define CLIENT_HPP

# include "ft_irc.hpp"

class Client
{

	public:

		Client();
		Client(int socket);
		Client( Client const & src );
		~Client();
		Client &		operator=( Client const & rhs );
		int 			getSocket() const;
		std::string 	getNickname() const;
		std::string 	getUsername() const;
		void 			setSocket(int socket);
		void 			setNickname(const std::string& nickname);
		void 			setUsername(const std::string& username);
	private:
		int 			_socket;
    	std::string		_nickname;
    	std::string 	_username;
};

std::ostream &			operator<<( std::ostream & o, Client const & i );

#endif /* ********************************************************** CLIENT_H */