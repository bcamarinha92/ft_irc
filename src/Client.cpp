#include "../inc/Client.hpp"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

Client::Client() {}

Client::Client(int socket): _lastAction(std::time(0)), _pingCount(0)
{
    _clientAddrLen = sizeof(_clientAddr);
    _clientSocket = accept(socket, (sockaddr*)&_clientAddr, &_clientAddrLen);
    if (_clientSocket < 0)
        throw std::invalid_argument("accept");
    _client_ip = inet_ntoa(_clientAddr.sin_addr);
	_hostname = gethostbyaddr((const void *)&_clientAddr.sin_addr, sizeof(_clientAddr.sin_addr), AF_INET);
    setNonBlocking(_clientSocket);
    clientPollfd.fd = _clientSocket;
    clientPollfd.events = POLLIN;
	this->_username = "";
	this->_realname = "";
}

Client::Client(const Client& src)
{
	this->_clientSocket = src._clientSocket;
	this->_nickname = src._nickname;
	this->_username = src._username;
	this->_realname = src._realname;
	this->_clientAddr = src._clientAddr;
	this->_clientAddrLen = src._clientAddrLen;
	this->_client_ip = src._client_ip;
	this->clientPollfd = src.clientPollfd;
    this->_lastAction = src._lastAction;
	this->channels = src.channels;
	this->_hostname = src._hostname;
	this->_pingCount = src._pingCount;
}

/*
** -------------------------------- DESTRUCTOR --------------------------------
*/

Client::~Client() {}

/*
** --------------------------------- OVERLOAD ---------------------------------
*/

Client&				Client::operator=(Client const& rhs)
{
	if (this != &rhs)
	{
		this->_clientSocket = rhs._clientSocket;
		this->_nickname = rhs._nickname;
		this->_username = rhs._username;
		this->_realname = rhs._realname;
		this->_clientAddr = rhs._clientAddr;
		this->_clientAddrLen = rhs._clientAddrLen;
		this->_client_ip = rhs._client_ip;
		this->clientPollfd = rhs.clientPollfd;
		this->channels = rhs.channels;
		this->_hostname = rhs._hostname;
		this->_lastAction = rhs._lastAction;
		this->_pingCount = rhs._pingCount;
	}
	return *this;
}

std::ostream&			operator<<(std::ostream& o, Client const& i)
{
	(void)i;
	//o << "Value = " << i.getValue();
	return o;
}


/*
** --------------------------------- METHODS ----------------------------------
*/

int 		Client::getSocket() const
{
	return (this->_clientSocket);
}

int 		Client::getPingCount() const
{
	return (this->_pingCount);
}


std::string	Client::getNickname() const
{
    return (this->_nickname);
}

sockaddr_in	Client::getclientAddr() const
{
    return(this->_clientAddr);
}

std::string Client::getRealname() const
{
	return(this->_realname);
}

socklen_t   Client::getclientAddrLen() const
{
    return(this->_clientAddrLen);
}

std::string Client::getUsername() const
{
    return (this->_username);
}

std::string Client::getHostname() const
{
    return (this->_hostname->h_name);
}

std::time_t		Client::getLastAction() const
{
    return (this->_lastAction);
}

void            Client::setSocket(int socket)
{
    _clientSocket = socket;
}

void 		Client::setNickname(const std::string& nickname)
{
    _nickname = nickname;
}

void 		Client::setUsername(const std::string& username)
{
    _username = username;
}

void 		Client::setRealname(const std::string& realname)
{
    _realname = realname;
}

void 		Client::incPingCount()
{
    _pingCount++;
}

void 		Client::resetPingCount()
{
    _pingCount = 0;
}

void            Client::setLastAction()
{
    _lastAction = std::time(0);
}

void		Client::addChannel(const Channel& channel)
{
	this->channels[channel.getName()] = channel;
}

void		Client::rmChannel(std::string channelName)
{
	this->channels.erase(channelName);
}

/*
** --------------------------------- ACCESSOR ---------------------------------
*/


/* ************************************************************************** */
