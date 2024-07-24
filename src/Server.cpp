#include "../inc/Server.hpp"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

Server::Server(int port, std::string password): _port(port), _password(password), _creationTime(std::time(0))
{
	_serverSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (_serverSocket < 0)
		throw std::invalid_argument("socket");
	std::memset(&_serverAddr, 0, sizeof(_serverAddr));
    _serverAddr.sin_family = AF_INET;
    _serverAddr.sin_addr.s_addr = INADDR_ANY;
    _serverAddr.sin_port = htons(_port);
	_hostIP = inet_ntoa(_serverAddr.sin_addr);

	_host = gethostbyname(_hostIP.c_str());
    if (_host == NULL) {
        std::cerr << "gethostbyname() failed" << std::endl;
    }
	_hostname = _host->h_name;
	int enable = 1;
	if (setsockopt(_serverSocket, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) < 0)
	{
		close(_serverSocket);
		throw std::runtime_error("setsockopt");
	}
	if (bind(_serverSocket, (sockaddr*)&_serverAddr, sizeof(_serverAddr)) < 0)
	{
        close(_serverSocket);
        throw std::invalid_argument("bind");
    }
    if (listen(_serverSocket, MAX_FD) < 0)
	{
        close(_serverSocket);
        throw std::invalid_argument("socket");
    }
	setNonBlocking(_serverSocket);
	serverPollfd.fd = _serverSocket;
    serverPollfd.events = POLLIN;
    pollfds.push_back(serverPollfd);
}

Server::Server(const Server& src)
{
	this->_host = src._host;
	this->_port = src._port;
	this->_hostIP = src._hostIP;
	this->_hostname = src._hostname;
	this->_password = src._password;
	this->_serverAddr = src._serverAddr;
	this->_serverSocket = src._serverSocket;
	this->_creationTime = src._creationTime;
	this->pollfds = src.pollfds;
	this->clients = src.clients;
	this->channels = src.channels;
	this->serverPollfd = src.serverPollfd;
}

/*
** -------------------------------- DESTRUCTOR --------------------------------
*/

Server::~Server() {}

/*
** --------------------------------- OVERLOAD ---------------------------------
*/

Server			&Server::operator=(Server const &rhs)
{
	if (this != &rhs)
	{
		this->_host = rhs._host;
		this->_port = rhs._port;
		this->_hostIP = rhs._hostIP;
		this->_hostname = rhs._hostname;
		this->_password = rhs._password;
		this->_serverAddr = rhs._serverAddr;
		this->_serverSocket = rhs._serverSocket;
		this->_creationTime = rhs._creationTime;
		this->pollfds = rhs.pollfds;
		this->clients = rhs.clients;
		this->channels = rhs.channels;
		this->serverPollfd = rhs.serverPollfd;
	}
	return *this;
}

std::ostream	&operator<<(std::ostream &o, Server const &i)
{
	//o << "Value = " << i.getValue();
	(void)i;
	return o;
}

/*
** --------------------------------- METHODS ----------------------------------
*/

int						Server::getPort() const
{
	return (this->_port);
}

std::string				Server::getPassword() const
{
	return (this->_password);
}
sockaddr_in				Server::getServerAddr() const
{
	return (this->_serverAddr);
}

int						Server::getServerSocket() const
{
	return (this->_serverSocket);
}

std::time_t				Server::getCreationDate() const
{
	return (this->_creationTime);
}

std::string				Server::getHostname() const
{
	return (this->_hostname);
}

void					Server::setPort(int port)
{
	this->_port=port;
}

void					Server::setPassword(std::string	password)
{
	this->_password = password;
}

void					Server::setServerAddr(sockaddr_in addr)
{
	this->_serverAddr = addr;
}

void					Server::setServerSocket(int skt)
{
	this->_serverSocket = skt;
}

void					Server::addClient(Client &user)
{
	this->pollfds.push_back(user.clientPollfd);
	this->clients[user.getSocket()] = user;
}

void					Server::rmClient(int clientSocket, int i)
{
	close(clientSocket);
	this->pollfds.erase(this->pollfds.begin() + i);
	this->clients.erase(clientSocket);
}

void					Server::addChannel(Channel &channel)
{
	this->channels[channel.getName()] = channel;
}

void					Server::rmChannel(std::string channelName)
{
	this->channels.erase(channelName);
}

std::string				Server::getNickByFd(int fd) const
{
    std::map<int, Client>::const_iterator it;

	it = this->clients.find(fd);
    if (it != this->clients.end())
	    return (*it).second.getNickname();
    return "";
}

void					Server::setNickByFd(int fd, std::string nickname)
{
	std::map<int, Client>::iterator it;

	it = this->clients.find(fd);
	if (it != this->clients.end())
    	(*it).second.setNickname(nickname);
}

const Client&			Server::getClientByFd(int socket) const
{
	std::map<int, Client>::const_iterator it = clients.find(socket);
    if (it != clients.end())
        return it->second;
    else
        throw std::runtime_error("Client not found");
}

void					Server::activateChannelMode(std::string const& chn, char mode, int sender, bool join)
{
	if (this->channels[chn].activateMode(mode, sender, join))
	{
		std::string const	msg = ":"+ this->getNickByFd(sender) + " MODE " + chn + " +" + std::string(1, mode);
		sendMessage(sender, this->channels[chn].getChannelClientsFds(), msg, ERR4, true);
	}
}

void					Server::deactivateChannelMode(std::string const& chn, char mode, int sender)
{
	if (this->channels[chn].deactivateMode(mode, sender))
	{
		std::string const	msg = ":"+ this->getNickByFd(sender) + " MODE " + chn + " -" + std::string(1, mode);
		sendMessage(sender, this->channels[chn].getChannelClientsFds(), msg, ERR5, true);
	}
}

/*
** --------------------------------- ACCESSOR ---------------------------------
*/

/* ************************************************************************** */
