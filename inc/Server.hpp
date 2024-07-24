#ifndef SERVER_HPP
# define SERVER_HPP

# include "ft_irc.hpp"

class Client;

class Channel;

class Server
{

	public:
		//construtores
		Server(int port, std::string password);
		Server(Server const& src);
		~Server();

		//overload
		Server&					operator=(Server const& rhs);

		//metodos
		int						getPort() const;
		std::string				getPassword() const;
		sockaddr_in				getServerAddr() const;
		int						getServerSocket() const;
		std::time_t				getCreationDate() const;
		std::string				getNickByFd(int fd) const;
		std::string				getHostname() const;
		void					setNickByFd(int fd, std::string nickname);
		void					setPort(int port);
		void					setPassword(std::string password);
		void					setServerAddr(sockaddr_in addr);
		void					setServerSocket(int skt);
		void					addClient(Client &user);
		void					rmClient(int clientSocket, int i);
		void					addChannel(Channel &channel);
		void					rmChannel(std::string channelName);
		const Client&			getClientByFd(int socket) const;
		void					printChannelModes(int sender, std::string channel);
		void					activateChannelMode(std::string const& chn, char mode, int sender, bool join);
		void					deactivateChannelMode(std::string const& chn, char mode, int sender);

		//atributos
		std::vector<pollfd> 			pollfds;
		std::map<int, Client> 			clients;
		std::map<std::string, Channel>	channels;
		pollfd 							serverPollfd;

	private:
		//atributos
		int						_port;
		std::string				_password;
		sockaddr_in 			_serverAddr;
		int						_serverSocket;
		struct hostent*			_host;
		std::string				_hostIP;
		std::string				_hostname;
		std::time_t				_creationTime;

};

std::ostream				&operator<<(std::ostream &o, Server const &i);

#endif /* ********************************************************** SERVER_H */
