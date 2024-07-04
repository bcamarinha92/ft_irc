#include "../inc/Message.hpp"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

Message::Message()
{
}

Message::Message( const Message & src )
{
	*this = src;
}

Message::Message( char *buffer)
{
	this->_buffer = buffer;
}

/*
** -------------------------------- DESTRUCTOR --------------------------------
*/

Message::~Message()
{
}


/*
** --------------------------------- OVERLOAD ---------------------------------
*/

Message &				Message::operator=( Message const & rhs )
{
	if ( this != &rhs )
	{
		this->_buffer = rhs.get_buffer();
	}
	return *this;
}

std::ostream &			operator<<( std::ostream & o, Message const & i )
{
	o << i.get_buffer();
	return o;
}


/*
** --------------------------------- METHODS ----------------------------------
*/

std::string Message::get_buffer() const
{
	return (_buffer);
}

void Message::set_buffer(std::string buffer)
{
	this->_buffer = buffer;
}

/*
** --------------------------------- ACCESSOR ---------------------------------
*/


/* ************************************************************************** */