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
	if (_host == NULL)
		throw std::runtime_error("gethostbyname() failed");
	char temp[256];
	if (gethostname(temp, sizeof(temp)) != 0)
    	throw std::runtime_error("gethostname() failed");
	_hostname = temp;
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
	this->modesParam['i'].first = 0;
	this->modesParam['i'].second = 0;
	this->modesParam['k'].first = 1;
	this->modesParam['k'].second = 0;
	this->modesParam['l'].first = 1;
	this->modesParam['l'].second = 0;
	this->modesParam['n'].first = 0;
	this->modesParam['n'].second = 0;
	this->modesParam['o'].first = 1;
	this->modesParam['o'].second = 1;
	this->modesParam['t'].first = 0;
	this->modesParam['t'].second = 0;
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
	this->modesParam = src.modesParam;
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
		this->modesParam = rhs.modesParam;
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

std::string				Server::getNickByFd(int fd) const
{
    std::map<int, Client>::const_iterator it;

	it = this->clients.find(fd);
    if (it != this->clients.end())
	    return (*it).second.getNickname();
    return "";
}

std::string				Server::getHostname() const
{
	return (this->_hostname);
}

Client&					Server::getClientByFd(int socket)
{
	std::map<int, Client>::iterator it = clients.find(socket);
    if (it != clients.end())
        return it->second;
    else
        throw std::runtime_error("Client not found");
}

int						Server::getFdFromNick(std::string nickname)
{
	std::map<int, Client>::iterator it = this->clients.begin();

	for (; it != this->clients.end(); ++it)
		if (it->second.getNickname() == nickname)
			return it->second.getSocket();
	return -1;
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

void					Server::setNickByFd(int fd, std::string nickname)
{
	std::map<int, Client>::iterator it;

	it = this->clients.find(fd);
	if (it != this->clients.end())
    	(*it).second.setNickname(nickname);
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

void					Server::rmClient(int clientSocket)
{
	close(clientSocket);
	for (size_t i = 0; i < pollfds.size(); ++i)
        if (pollfds[i].fd == clientSocket)
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

void					Server::activateChannelMode(std::string const& chn, char mode, int sender, bool join, std::string param)
{
	if (this->channels[chn].checkOperatorRole(sender))
	{
		if (mode == 'o')
		{
			int fd = getFdFromNick(param);
			if (fd != -1)
			{
				if (this->channels[chn].operators.find(fd) == this->channels[chn].operators.end())
				{
					this->channels[chn].activateMode(mode, sender, join);
					sendMessageAll(sender, this->channels[chn].getChannelClientsFds(), \
					RPL_CHANNELMODEISACT(this->getNickByFd(sender), chn, mode, \
						  this->getNickByFd(fd)), ERRAOM, false);
					this->channels[chn].addOperator(this->getClientByFd(fd));
				}
			}
		}
		else if (mode == 'l')
		{
			std::stringstream	ss(param);
			ssize_t				limit;

			ss >> limit;
			if (!ss.fail() && limit > 0)
			{
				if (limit < 3001)
				{
					this->channels[chn].activateMode(mode, sender, join);
					this->channels[chn].setChannelUserLimit(limit);
					sendMessageAll(sender, this->channels[chn].getChannelClientsFds(), \
					RPL_CHANNELMODEISACT(this->getNickByFd(sender), chn, mode, param), ERRALM, false);
				}
				else
					sendMessage(sender, RPL_TOOHIGHLIMIT(this->getHostname(), \
										  this->getNickByFd(sender), chn), ERRLTH);
			}
		}
		else if (mode == 'k')
		{
			if (param != "")
			{
				this->channels[chn].activateMode(mode, sender, join);
				this->channels[chn].setChannelKey(param);
				sendMessageAll(sender, this->channels[chn].getChannelClientsFds(), \
				   RPL_CHANNELMODEISACT(this->getNickByFd(sender), chn, mode, param), ERRACK, false);
			}
		}
		else if (!this->channels[chn].checkChannelMode(mode) && this->channels[chn].activateMode(mode, sender, join))
		{
			sendMessageAll(sender, this->channels[chn].getChannelClientsFds(), \
				  ACTMODE(this->getNickByFd(sender), chn, std::string(1, mode)), ERRAM, false);
		}
	}
	else
		sendMessage(sender, ERR_CHANOPRIVSNEEDED(this->getHostname(), this->getNickByFd(sender), chn), ERR482);
}

void					Server::deactivateChannelMode(std::string const& chn, char mode, int sender, std::string param)
{
	if (this->channels[chn].checkOperatorRole(sender))
	{
		if (mode == 'o')
		{
			int fd = getFdFromNick(param);
			if (fd != -1)
			{
				if (this->channels[chn].operators.find(fd) != this->channels[chn].operators.end())
				{
					this->channels[chn].deactivateMode(mode, sender);
					sendMessageAll(sender, this->channels[chn].getChannelClientsFds(), \
					RPL_CHANNELMODEISDEACT(this->getNickByFd(sender), chn, mode, \
							this->getNickByFd(fd)), ERRDOM, false);
					this->channels[chn].rmOperator(fd);
				}
			}
		}
		else if (mode == 'l' && this->channels[chn].checkChannelMode(mode))
		{
			this->channels[chn].deactivateMode(mode, sender);
			this->channels[chn].setChannelUserLimit(-1);
			sendMessageAll(sender, this->channels[chn].getChannelClientsFds(), \
				  DEACTMODE(this->getNickByFd(sender), chn, std::string(1, mode)), ERRDLM, false);
		}
		else if (mode == 'k' && this->channels[chn].checkChannelMode(mode))
		{
			this->channels[chn].deactivateMode(mode, sender);
			this->channels[chn].setChannelKey("");
			sendMessageAll(sender, this->channels[chn].getChannelClientsFds(), \
				  DEACTMODE(this->getNickByFd(sender), chn, mode), ERRDCK, false);
		}
		else if (this->channels[chn].checkChannelMode(mode) && this->channels[chn].deactivateMode(mode, sender))
			sendMessageAll(sender, this->channels[chn].getChannelClientsFds(), \
				  DEACTMODE(this->getNickByFd(sender), chn, std::string(1, mode)), ERRDM, false);
	}
	else
		sendMessage(sender, ERR_CHANOPRIVSNEEDED(this->getHostname(), this->getNickByFd(sender), chn), ERR482);
}

/*
** --------------------------------- ACCESSOR ---------------------------------
*/

/* ************************************************************************** */
