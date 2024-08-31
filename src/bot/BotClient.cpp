#include "../../inc/BotClient.hpp"
#include <cstdio>

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

BotClient::BotClient(std::string serverIP, int port, std::string pass)
{
	_ipServer = serverIP;
	_port = port;
	_password = pass;
	_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (_socket < 0)
		throw std::invalid_argument("socket");
	_server.sin_family = AF_INET;
    _server.sin_port = htons(_port);
    inet_pton(AF_INET, _ipServer.c_str(), &_server.sin_addr);
	std::cout << "ola\n";
    if (connect(_socket, (struct sockaddr *)&_server, sizeof(_server)) < 0)
		throw std::invalid_argument("connect");
	std::cout << "ola2\n";
	this->_nickname1 = "bbc";
	this->_nickname2 = "botzasso";
	this->_nickname3 = "botinho";
	this->_username = "bot";
	this->_realname = "bot";
}

BotClient::BotClient(const BotClient& src)
{
	this->_socket = src._socket;
	this->_nickname1 = src._nickname1;
	this->_nickname2 = src._nickname2;
	this->_nickname3 = src._nickname3;
	this->_username = src._username;
	this->_realname = src._realname;
	this->_port = src._port;
	this->_server = src._server;
	this->_ipServer = src._ipServer;
	this->_password = src._password;
}

/*
** -------------------------------- DESTRUCTOR --------------------------------
*/

BotClient::~BotClient()
{

}

/*
** --------------------------------- OVERLOAD ---------------------------------
*/

/*BotClient&				BotClient::operator=(BotClient const& rhs)
{
	if (this != &rhs)
	{
		this->_socket = rhs._socket;
		this->_nickname = rhs._nickname;
		this->_username = rhs._username;
		this->_realname = rhs._realname;
		this->_botAddr = rhs._botAddr;
		this->_botAddrLen = rhs._botAddrLen;
		this->_bot_ip = rhs._bot_ip;
		this->botPollfd = rhs.botPollfd;
		this->_hostname = rhs._hostname;
		this->_lastAction = rhs._lastAction;
		this->_pingCount = rhs._pingCount;
	    this->_pwdStatus = rhs._pwdStatus;
	}
	return *this;
}*/

/*
** --------------------------------- METHODS ----------------------------------
*/

int 		BotClient::getSocket() const
{
	return (this->_socket);
}

std::string	BotClient::getNickname() const
{
    return (this->_nickname1);
}

std::string BotClient::getRealname() const
{
	return(this->_realname);
}

std::string BotClient::getUsername() const
{
    return (this->_username);
}

std::string	BotClient::getPassword() const
{
	return (this->_password);
}
