#ifndef SERVER_HPP
# define SERVER_HPP

# include "ft_irc.hpp"

class Client;

class Channel;

typedef std::pair<int, int>	modesPair;

typedef std::pair<int, int>	modesPair;

class Server
{

	public:
		//Constructors
		//Constructors
		Server(int port, std::string password);
		Server(Server const& src);

		//Destructors

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
		Client&							getClientByFd(int socket);
		int								getFdFromNick(std::string nickname);

		//Setters
		void							setPort(int port);
		void							setPassword(std::string password);
		void							setServerAddr(sockaddr_in addr);
		void							setServerSocket(int skt);
		void							setNickByFd(int fd, std::string nickname);

		//Methods
		void							addClient(Client &user);
		void							rmClient(int clientSocket, int i);
		void							rmClient(int clientSocket);
		void							addChannel(Channel &channel);
		void							rmChannel(std::string channelName);
		void							activateChannelMode(std::string const& chn, char mode, int sender, bool join, std::string param);
		void							deactivateChannelMode(std::string const& chn, char mode, int sender, std::string param);

		//Attributes
		std::vector<pollfd> 			pollfds;
		std::map<int, Client> 			clients;
		std::map<std::string, Channel>	channels;
		pollfd 							serverPollfd;
		std::map<char, modesPair>		modesParam;
		int								botSocket;
		unsigned long					max_len;

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
