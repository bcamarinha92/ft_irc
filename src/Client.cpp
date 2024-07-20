#include "../inc/Client.hpp"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

Client::Client()
{
}

Client::Client(int socket)
{
    _clientAddrLen = sizeof(_clientAddr);
    _clientSocket = accept(socket, (sockaddr*)&_clientAddr, &_clientAddrLen);
    if (_clientSocket < 0)
        throw std::invalid_argument("accept");
    _client_ip = inet_ntoa(_clientAddr.sin_addr);
    setNonBlocking(_clientSocket);
    clientPollfd.fd = _clientSocket;
    clientPollfd.events = POLLIN;
}

Client::Client( const Client & src )
{
	this->_clientSocket = src._clientSocket;
	this->_nickname = src._nickname;
	this->_username = src._username;
}


/*
** -------------------------------- DESTRUCTOR --------------------------------
*/

Client::~Client()
{
}


/*
** --------------------------------- OVERLOAD ---------------------------------
*/

Client &				Client::operator=( Client const & rhs )
{
	(void)rhs;
	//if ( this != &rhs )
	//{
		//this->_value = rhs.getValue();
	//}
	return *this;
}


std::ostream& operator<<(std::ostream& os, const Client& client) 
{
    os << "Nickname: " << client.getNickname() << std::endl;
    os << "Username: " << client.getUsername() << std::endl;
    os << "Client Socket: " << client.getSocket() << std::endl;
    return os;
}


/*
** --------------------------------- METHODS ----------------------------------
*/

    int Client::getSocket() const
	{
        return (this->_clientSocket);
    }

    std::string Client::getNickname() const
	{
        return (this->_nickname);
    }

    sockaddr_in Client::getclientAddr() const
    {
        return(this->_clientAddr);
    }

    socklen_t   Client::getclientAddrLen() const
    {
        return(this->_clientAddrLen);
    }

    std::string Client::getUsername() const
	{
        return (this->_username);
    }

    void Client::setSocket(int socket)
	{
        _clientSocket = socket;
    }

    void Client::setNickname(const std::string& nickname)
	{
        _nickname = nickname;
    }

    void Client::setUsername(const std::string& username)
	{
        _username = username;
    }

/*
** --------------------------------- ACCESSOR ---------------------------------
*/


/* ************************************************************************** */
