#include "../inc/Message.hpp"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

Message::Message() {}

Message::Message(const Message& src)
{
	*this = src;
}

Message::Message(std::string buffer, int sender): _sender(sender)
{
	this->_buffer = removeSpaces(buffer);
	this->_command = toUpper(get_buffer_command(removeSpaces(buffer)));
	this->_parameters = get_buffer_parameters(removeSpaces(buffer));
	this->_destination = getChannelFromBuffer(removeSpaces(buffer));
}

/*
** -------------------------------- DESTRUCTOR --------------------------------
*/

Message::~Message() {}

/*
** --------------------------------- OVERLOAD ---------------------------------
*/

Message&				Message::operator=(Message const& rhs)
{
	if (this != &rhs)
	{
		this->_buffer = rhs.get_buffer();
		this->_sender = rhs.get_sender();
		this->_target = rhs.get_target();
		this->_command = rhs.get_command();
		this->_parameters = rhs.get_parameters();
		this->_destination = rhs.get_destination();
	}
	return *this;
}

std::ostream&			operator<<(std::ostream& o, Message const& i)
{
	o << "Buffer: " << i.get_buffer();
	o << "Command: " << i.get_command() << std::endl;
	o << "Parameters: ";

	std::vector<std::string>	param = i.get_parameters();
	int							length = param.size();

	for(int i = 0; i < length; i++)
		std::cout << param[i] << ',' << std::endl;

	o << "Sender: " << i.get_sender() << std::endl;
	o << "Destination: " << i.get_destination() << std::endl;
	return o;
}

/*
** --------------------------------- METHODS ----------------------------------
*/

std::string					Message::get_buffer() const
{
	return (_buffer);
}

int							Message::get_sender() const
{
	return(_sender);
}

std::string					Message::get_destination() const
{
	return(_destination);
}

std::string					Message::get_command() const
{
	return (_command);
}

std::vector<std::string>	Message::get_parameters() const
{
	return (_parameters);
}

std::string					Message::get_target() const
{
	return (_target);
}
void						Message::set_buffer(std::string buffer)
{
	this->_buffer = buffer;
}

void						Message::set_command(std::string command)
{
	this->_command = command;
}

void						Message::set_target(std::string target)
{
	this->_target = target;
}

/*
** --------------------------------- ACCESSOR ---------------------------------
*/


/* ************************************************************************** */
