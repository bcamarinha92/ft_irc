#include "../inc/Message.hpp"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

Message::Message() {}

Message::Message(const Message& src) {}

/*
** -------------------------------- DESTRUCTOR --------------------------------
*/

Message::~Message() {}

/*
** --------------------------------- OVERLOAD ---------------------------------
*/

Message&				Message::operator=(Message const& rhs)
{
	//if ( this != &rhs )
	//{
		//this->_value = rhs.getValue();
	//}
	return *this;
}

std::ostream&			operator<<(std::ostream& o, Message const& i)
{
	//o << "Value = " << i.getValue();
	return o;
}


/*
** --------------------------------- METHODS ----------------------------------
*/


/*
** --------------------------------- ACCESSOR ---------------------------------
*/


/* ************************************************************************** */
