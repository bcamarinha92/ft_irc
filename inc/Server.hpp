#ifndef SERVER_HPP
# define SERVER_HPP

# include "ft_irc.hpp"

class Client;

class Channel;

class Server
{

	public:
		//Constructors
		Server(int port, std::string password);
		Server(Server const& src);

		//Destructors
		~Server();

		//Overloads
		Server&							operator=(Server const& rhs);

		//Getters
		int								getPort() const;
		std::string						getPassword() const;
		sockaddr_in						getServerAddr() const;
		int								getServerSocket() const;
		std::time_t						getCreationDate() const;
		std::string						getNickByFd(int fd) const;
		std::string						getHostname() const;
		const Client&					getClientByFd(int socket) const;

		//Setters
		void							setNickByFd(int fd, std::string nickname);
		void							setPort(int port);
		void							setPassword(std::string password);
		void							setServerAddr(sockaddr_in addr);
		void							setServerSocket(int skt);

		//Methods
		void							addClient(Client &user);
		void							rmClient(int clientSocket, int i);
		void							addChannel(Channel &channel);
		void							rmChannel(std::string channelName);
		void							activateChannelMode(std::string const& chn, char mode, int sender, bool join);
		void							deactivateChannelMode(std::string const& chn, char mode, int sender);

		//Attributes
		std::vector<pollfd> 			pollfds;
		std::map<int, Client> 			clients;
		std::map<std::string, Channel>	channels;
		pollfd 							serverPollfd;

	private:
		//Attributes
		int								_port;
		std::string						_password;
		sockaddr_in 					_serverAddr;
		int								_serverSocket;
		struct hostent*					_host;
		std::string						_hostIP;
		std::string						_hostname;
		std::time_t						_creationTime;

};

std::ostream				&operator<<(std::ostream &o, Server const &i);

#endif /* ********************************************************** SERVER_H */
