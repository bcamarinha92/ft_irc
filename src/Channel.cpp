#include "../inc/Channel.hpp"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

Channel::Channel(): _launch(true)
{
	this->prepareModes();
}

Channel::Channel(std::string name): _createdAt(std::time(0)), _launch(true)
{
	this->_name = name;
	this->prepareModes();
}

Channel::Channel(const Channel& src)
{
	this->_name = src._name;
	this->_topic = src._topic;
	this->_modes = src._modes;
	this->_createdAt = src._createdAt;
	this->_launch = src._launch;
	this->members = src.members;
	this->operators = src.operators;
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
	if ( this != &rhs )
	{
		this->_name = rhs._name;
		this->_topic = rhs._topic;
		this->_modes = rhs._modes;
		this->_launch = rhs._launch;
		this->_createdAt = rhs._createdAt;
		this->members = rhs.members;
		this->operators = rhs.operators;
	}
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

std::string			Channel::getName() const
{
	return(this->_name);
}

std::string			Channel::getTopic() const
{
	return(this->_topic);
}

std::time_t			Channel::getCreatedAtTime() const
{
	return (this->_createdAt);
}

bool				Channel::getLaunch() const
{
	return (this->_launch);
}

void				Channel::setName(std::string name)
{
	this->_name=name;
}

void				Channel::setTopic(std::string topic)
{
	this->_topic = topic;
}

void				Channel::addClient(const Client& client)
{
	this->members[client.getSocket()] = client;
}

void				Channel::rmClient(int clientSocket)
{
	this->members.erase(clientSocket);
}

void				Channel::addOperator(const Client& client)
{
	this->operators[client.getSocket()] = client;
}

void				Channel::rmOperator(int clientSocket)
{
	this->operators.erase(clientSocket);
}

void				Channel::prepareModes()
{
	for (char i = 'a'; i < 'z' + 1; i++)
		this->_modes[i] = false;
}

bool				Channel::checkOperatorRole(int fd)
{
	return this->operators.find(fd) != this->operators.end();
}

bool				Channel::activateMode(char mode, int sender, bool join)
{
	if (this->checkOperatorRole(sender) || join)
	{
		this->_modes[mode] = true;
		return true;
	}
	return false;
}

bool				Channel::deactivateMode(char mode, int sender)
{
	if (this->checkOperatorRole(sender))
	{
		this->_modes[mode] = false;
		return true;
	}
	return false;
}

std::string			Channel::getChannelModes()
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

std::vector<int>	Channel::getChannelClientsFds()
{
	std::map<int, Client>::iterator	it = this->members.begin();
	std::vector<int>				fds;

	for (; it != this->members.end(); ++it)
		fds.push_back(it->first);
	return fds;
}

void				Channel::switchLaunch()
{
	this->_launch = !this->_launch;
}

/*
** --------------------------------- ACCESSOR ---------------------------------
*/


/* ************************************************************************** */
