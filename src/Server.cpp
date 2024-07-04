#include "../inc/Server.hpp"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

Server::Server(int port, std::string password): _port(port), _password(password)
{
	_serverSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (_serverSocket < 0) 
		throw std::invalid_argument("socket");
	std::memset(&_serverAddr, 0, sizeof(_serverAddr));
    _serverAddr.sin_family = AF_INET;
    _serverAddr.sin_addr.s_addr = INADDR_ANY;
    _serverAddr.sin_port = htons(_port);

	if (bind(_serverSocket, (sockaddr*)&_serverAddr, sizeof(_serverAddr)) < 0)
	{    
        close(_serverSocket);
        throw std::invalid_argument("bind");
    }
    if (listen(_serverSocket, MAX_CLIENTS) < 0) 
	{
        close(_serverSocket);
        throw std::invalid_argument("socket");
    }
	setNonBlocking(_serverSocket);
	serverPollfd.fd = _serverSocket;
    serverPollfd.events = POLLIN;
    pollfds.push_back(serverPollfd);
}

Server::Server( const Server & src )
{
	(void)src;
}


/*
** -------------------------------- DESTRUCTOR --------------------------------
*/

Server::~Server()
{
}


/*
** --------------------------------- OVERLOAD ---------------------------------
*/

Server &				Server::operator=( Server const & rhs )
{
	//if ( this != &rhs )
	//{
		//this->_value = rhs.getValue();
	//}
	(void)rhs;
	return *this;
}

std::ostream &			operator<<( std::ostream & o, Server const & i )
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

std::string				Server::getPAssword() const
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

/*
** --------------------------------- ACCESSOR ---------------------------------
*/


/* ************************************************************************** */