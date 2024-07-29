#include "../inc/Channel.hpp"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

Channel::Channel(): _launch(true), _ulimit(-1), _key("")
{
	this->prepareModes();
}

Channel::Channel(std::string name): _createdAt(std::time(0)), _launch(true), _ulimit(-1), _key("")
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
	this->_invites = src._invites;
	this->_ulimit = src._ulimit;
	this->_key = src._key;
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
		this->_invites = rhs._invites;
		this->_ulimit = rhs._ulimit;
		this->_key = rhs._key;
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

std::vector<int>	Channel::getChannelInvites() const
{
	return (this->_invites);
}

size_t				Channel::getChannelUserLimit() const
{
	return (this->_ulimit);
}

std::string			Channel::getChannelKey() const
{
	return (this->_key);
}

void				Channel::setName(std::string name)
{
	this->_name=name;
}

void				Channel::setTopic(std::string topic)
{
	this->_topic = topic;
}

void				Channel::switchLaunch()
{
	this->_launch = !this->_launch;
}

void				Channel::setChannelUserLimit(size_t limit)
{
	this->_ulimit = limit;
}

void				Channel::setChannelKey(std::string key)
{
	this->_key = key;
}

void				Channel::prepareModes()
{
	this->_modes['i'] = false;
	this->_modes['k'] = false;
	this->_modes['l'] = false;
	this->_modes['n'] = false;
	this->_modes['t'] = false;
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

bool				Channel::checkOperatorRole(int fd)
{
	return this->operators.find(fd) != this->operators.end();
}

bool				Channel::checkChannelMode(char mode)
{
	return (this->_modes[mode]);
}

void				Channel::addInvite(int fd)
{
	this->_invites.push_back(fd);
}

void				Channel::rmInvite(int fd)
{
	bool	found = false;
	size_t 	i = 0;

	for (; i < this->_invites.size(); ++i)
	{
		if (this->_invites[i] == fd)
		{
			found = true;
			break ;
		}
	}
	if (found)
		this->_invites.erase(this->_invites.begin() + i);
}

/*
** --------------------------------- ACCESSOR ---------------------------------
*/


/* ************************************************************************** */
