#include "../../inc/BotClient.hpp"

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
    if (connect(_socket, (struct sockaddr *)&_server, sizeof(_server)) < 0)
		throw std::invalid_argument("connect");

	this->_nickname1 = "bbc";
	this->_nickname2 = "botzasso";
	this->_nickname3 = "botinho";
	this->_username = "bot";
	this->_realname = "bot";

	nn = 1;
	msgs.push_back("I’m like motor oil: I can handle a lot of pressure, but don’t mess with my viscosity!");
    msgs.push_back("I love the smell of motor oil in the morning. Smells like... weekend projects!");
    msgs.push_back("Why did the car engine start a band? Because it loved heavy metal and greasy grooves!");
    msgs.push_back("If you can't stand the oil, get out of the garage!");
    msgs.push_back("I like my engines like I like my coffee: full of oil and ready to go!");
    msgs.push_back("When life gives you oil leaks, make garage puddles!");
    msgs.push_back("I don't always change my oil, but when I do, I make sure to spill it everywhere.");
    msgs.push_back("Oil is like duct tape for cars – if it moves and shouldn't, oil it!");
    msgs.push_back("You know you're a gearhead when your cologne smells like motor oil.");
    msgs.push_back("Just like oil in my engine, a little humor keeps me running smoothly!");
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
	close(this->_socket);
}

/*
** --------------------------------- OVERLOAD ---------------------------------
*/

BotClient&				BotClient::operator=(BotClient const& rhs)
{
	if (this != &rhs)
	{
		this->_socket = rhs._socket;
		this->_nickname1 = rhs._nickname1;
		this->_nickname2 = rhs._nickname2;
		this->_nickname3 = rhs._nickname3;
		this->_username = rhs._username;
		this->_realname = rhs._realname;
		this->_port = rhs._port;
		this->_server = rhs._server;
		this->_ipServer = rhs._ipServer;
		this->_password = rhs._password;
	}
	return *this;
}

/*
** --------------------------------- METHODS ----------------------------------
*/

int 		BotClient::getSocket() const
{
	return (this->_socket);
}

std::string	BotClient::getNickname() const
{
	if (nn == 1)
    	return (this->_nickname1);
	else if (nn == 2)
		return (this->_nickname2);
	else
		return (this->_nickname3);
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
