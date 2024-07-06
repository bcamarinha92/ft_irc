#ifndef SERVER_HPP
# define SERVER_HPP

# include "ft_irc.hpp"

class Client;

class Server
{

	public:
		//construtores
		Server(int port, std::string password);
		Server( Server const & src );
		~Server();

		//overload
		Server &				operator=( Server const & rhs );
		
		//metodos
		int						getPort() const;
		std::string				getPAssword() const;
		sockaddr_in				getServerAddr() const;
		int						getServerSocket() const;
		void					setPort(int port);
		void					setPassword(std::string password);
		void					setServerAddr(sockaddr_in addr);
		void					setServerSocket(int skt);
		void					addClient(Client &user);
		void					rmClient(int clientSocket, int i);
		
		//atributos
		std::vector<pollfd> 	pollfds;
   		std::map<int, Client> 	clients;
		pollfd 					serverPollfd;
	private:
		//atributos
		int						_port;
		std::string				_password;
		sockaddr_in 			_serverAddr;
		int						_serverSocket;

};

std::ostream &			operator<<( std::ostream & o, Server const & i );

#endif /* ********************************************************** SERVER_H */