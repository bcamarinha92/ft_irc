#ifndef MESSAGE_HPP
# define MESSAGE_HPP

# include <iostream>
# include <string>

class Message
{

	public:

		Message();
		Message( Message const & src );
		~Message();

		Message &		operator=( Message const & rhs );

	private:

};

std::ostream &			operator<<( std::ostream & o, Message const & i );

#endif /* ********************************************************* MESSAGE_H */