#ifndef MESSAGE_HPP
# define MESSAGE_HPP

# include "ft_irc.hpp"

class Message
{
	public:
		//Constructors
		Message();
		Message(Message const& src);
		Message(char *buffer, int sender);

		//Destructors
		~Message();

		//Getters
		std::string 				get_buffer() const;
		std::string 				get_command() const;
		std::vector<std::string>	get_parameters() const;
		int 						get_sender();
		std::string 				get_destination() const;

		//Setters
		void set_buffer(std::string buffer);
		void set_command(std::string command);

		Message &		operator=(Message const& rhs);

	private:
		std::string 				_buffer;
		std::string 				_command;
		std::vector<std::string>	_parameters;
		int							_sender;
		std::string					_destination;

};

std::ostream&			operator<<(std::ostream& o, Message const& i);

#endif /* ********************************************************* MESSAGE_H */
