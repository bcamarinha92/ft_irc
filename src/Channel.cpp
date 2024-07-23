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

Channel::Channel( const Channel & src )
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

Channel &				Channel::operator=( Channel const & rhs )
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

std::string			Channel::getName() const
{
	return(this->_name);
}

std::string			Channel::getTopic() const
{
	return(this->_topic);
}

std::string			Channel::getCreatedAtTime() const
{
	/*std::ctime(&this->_createdAt);
	std::ostringstream	oss;
	oss << this->_createdAt;
	return oss.str();*/
	char buffer[26];
	std::cout << this->_createdAt << std::endl;
    std::tm* tm_info = std::localtime(&this->_createdAt); // Convert to local time
    std::strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", tm_info); // Format time
	std::cout << std::string(buffer) << std::endl;
	return std::string(buffer);
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

std::string			Channel::printChannelModes()
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
	int	i = 0;
	for (; it != this->members.end(); ++it)
	{
		fds[i] = it->first;
		i++;
	}
	return fds;;
}

/*
** --------------------------------- ACCESSOR ---------------------------------
*/


/* ************************************************************************** */
