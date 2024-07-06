#ifndef CHANNEL_HPP
# define CHANNEL_HPP

# include "ft_irc.hpp"

class Client;

class Channel
{

	public:
		//construtores
		Channel();
		Channel( Channel const & src );
		~Channel();
		//overloads
		Channel &		operator=( Channel const & rhs );
		//metodos
		std::string		getName() const;
		std::string		getTopic() const;
		void			setName(std::string name);
		void			setTopic(std::string topic);		
		//atributos
	private:
		//atributos
		std::string				_name;
		std::string				_topic;
		std::map<int, Client>	_members;

};

std::ostream &			operator<<( std::ostream & o, Channel const & i );

#endif /* ********************************************************* CHANNEL_H */