#ifndef MESSAGE_HPP
# define MESSAGE_HPP

# include "ft_irc.hpp"

class Message
{
	public:
		//Constructors
		Message();
		Message(Message const& src);
		Message(std::string buffer, int sender);

		//Destructors
		~Message();

		//Overloads
		Message&					operator=(Message const& rhs);

		//Getters
		std::string 				get_buffer() const;
		std::string 				get_command() const;
		std::vector<std::string>	get_parameters() const;
		int 						get_sender() const;
		std::string 				get_destination() const;
		std::string					get_target() const;

		//Setters
		void						set_buffer(std::string buffer);
		void 						set_command(std::string command);
		void						set_target(std::string target);

	private:
		//Attributes
		std::string 				_buffer;
		std::string 				_command;
		std::vector<std::string>	_parameters;
		int							_sender;
		std::string					_destination;
		std::string					_target;

};

std::ostream&			operator<<(std::ostream& o, Message const& i);

#endif /* ********************************************************* MESSAGE_H */
