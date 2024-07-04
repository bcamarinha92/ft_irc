#ifndef MESSAGE_HPP
# define MESSAGE_HPP

# include "ft_irc.hpp"

class Message
{

	public:

		//Constructors
		Message();
		Message( Message const & src );
		Message( char *buffer);
		
		//Destructors
		~Message();

		//Getters
		std::string get_buffer() const;

		//Setters
		void set_buffer(std::string buffer);

		Message &		operator=( Message const & rhs );

	private:
		std::string _buffer;

};

std::ostream &			operator<<( std::ostream & o, Message const & i );

#endif /* ********************************************************* MESSAGE_H */