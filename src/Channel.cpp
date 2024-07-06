#include "../inc/Channel.hpp"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

Channel::Channel()
{
}

Channel::Channel( const Channel & src )
{
	(void)src;
}


/*
** -------------------------------- DESTRUCTOR --------------------------------
*/

Channel::~Channel()
{
}


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

std::ostream &			operator<<( std::ostream & o, Channel const & i )
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

/*
** --------------------------------- ACCESSOR ---------------------------------
*/


/* ************************************************************************** */