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

Message::Message(char *buffer, int sender): _sender(sender)
{
	this->_buffer = buffer;
	this->_command = get_buffer_command(buffer);
	this->_parameters = get_buffer_parameters(buffer);
	this->_destination = getChannelFromBuffer(std::string(buffer));
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
	o << "Buffer: " << i.get_buffer();
	o << "Command: " << i.get_command() << std::endl;
	o << "Parameters: ";

	std::vector<std::string> param = i.get_parameters();
	int length = param.size();

	for(int i = 0; i < length; i++) {
		std::cout << param[i] << ',' << std::endl;
	}
	return o;
}


/*
** --------------------------------- METHODS ----------------------------------
*/

std::string Message::get_buffer() const
{
	return (_buffer);
}

int			Message::get_sender()
{
	return(_sender);
}

std::string	Message::get_destination() const
{
	return(_destination);
}

void Message::set_buffer(std::string buffer)
{
	this->_buffer = buffer;
}

std::string Message::get_command() const
{
	return (_command);
}

void Message::set_command(std::string command)
{
	this->_command = command;
}

std::vector<std::string> Message::get_parameters() const
{
	return (_parameters);
}

/*
** --------------------------------- ACCESSOR ---------------------------------
*/


/* ************************************************************************** */