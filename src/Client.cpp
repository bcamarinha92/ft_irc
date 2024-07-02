#include "../inc/Client.hpp"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

Client::Client()
{
}

Client::Client(int socket) : _socket(socket)
{
}

Client::Client( const Client & src )
{
	this->_socket = src._socket;
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

std::ostream &			operator<<( std::ostream & o, Client const & i )
{
	(void)i;
	//o << "Value = " << i.getValue();
	return o;
}


/*
** --------------------------------- METHODS ----------------------------------
*/

    int Client::getSocket() const 
	{
        return (this->_socket);
    }

    std::string Client::getNickname() const 
	{
        return (this->_nickname);
    }

    std::string Client::getUsername() const 
	{
        return (this->_username);
    }

    void Client::setSocket(int socket) 
	{
        _socket = socket;
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