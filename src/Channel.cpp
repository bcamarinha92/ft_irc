#include "../inc/Channel.hpp"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

Channel::Channel()
{
	this->prepareModes();
}

Channel::Channel(std::string name)
{
	this->_name = name;
	this->prepareModes();
}

Channel::Channel(const Channel& src)
{
	(void)src;
}

/*
** -------------------------------- DESTRUCTOR --------------------------------
*/

Channel::~Channel() {}

/*
** --------------------------------- OVERLOAD ---------------------------------
*/

Channel&				Channel::operator=(Channel const& rhs)
{
	(void) rhs;
	//if ( this != &rhs )
	//{
		//this->_value = rhs.getValue();
	//}
	return *this;
}

std::ostream&			operator<<(std::ostream& o, Channel const& i)
{
	(void)i;
	//o << "Value = " << i.getValue();
	return o;
}

/*
** --------------------------------- METHODS ----------------------------------
*/

std::string		Channel::getName() const
{
	return(this->_name);
}

std::string		Channel::getTopic() const
{
	return(this->_topic);
}

void			Channel::setName(std::string name)
{
	this->_name=name;
}

void			Channel::setTopic(std::string topic)
{
	this->_topic = topic;
}

void			Channel::addClient(const Client& client)
{
	this->members[client.getSocket()] = client;
}

void			Channel::rmClient(int clientSocket)
{
	this->members.erase(clientSocket);
}

void			Channel::addOperator(const Client& client)
{
	this->operators[client.getSocket()] = client;
}

void			Channel::rmOperator(int clientSocket)
{
	this->operators.erase(clientSocket);
}

void			Channel::prepareModes()
{
	for (char i = 'a'; i < 'z' + 1; i++)
		this->_modes[i] = false;
}

bool			Channel::checkOperatorRole(int fd)
{
	return this->operators.find(fd) != this->operators.end();
}

bool			Channel::activateMode(char mode, int sender, bool join)
{
	if (this->checkOperatorRole(sender) || join)
	{
		this->_modes[mode] = true;
		return true;
	}
	return false;
}

bool			Channel::deactivateMode(char mode, int sender)
{
	if (this->checkOperatorRole(sender))
	{
		this->_modes[mode] = false;
		return true;
	}
	return false;
}

std::string		Channel::printChannelModes()
{
	std::stringstream	ss;
	ss << "+";
    for (std::map<char, bool>::iterator it = this->_modes.begin(); it != this->_modes.end(); ++it)
    {
		if (it->second == true)
			ss << it->first;
	}
	return ss.str();
}

void			Channel::sendMsgToChannelClients(std::string msg)
{
	(void)msg;
}

/*
** --------------------------------- ACCESSOR ---------------------------------
*/


/* ************************************************************************** */
