#ifndef CHANNEL_HPP
# define CHANNEL_HPP

# include "ft_irc.hpp"

class Client;

class Channel
{
	public:
		//construtores
		Channel();
		Channel(std::string name);
		Channel(Channel const& src);
		~Channel();
		//overloads
		Channel&				operator=(Channel const& rhs);

		//methods
		std::string				getName() const;
		std::string				getTopic() const;
		void					setName(std::string name);
		void					setTopic(std::string topic);
		void					addClient(const Client& client);
		void					rmClient(int clientSocket);
		void					addOperator(const Client& client);
		void					rmOperator(int clientSocket);
		void					prepareModes();
		bool					activateMode(char mode, int sender, bool join);
		bool					deactivateMode(char mode, int sender);
		bool					checkOperatorRole(int fd);
		std::string				printChannelModes();
		void					sendMsgToChannelClients(std::string msg);
		//attributes
		std::map<int, Client>	members;
		std::map<int, Client>	operators;

	private:
		//attributes
		std::string				_name;
		std::string				_topic;
		std::map<char, bool>	_modes;
};

std::ostream&			operator<<(std::ostream& o, Channel const& i);

#endif /* ********************************************************* CHANNEL_H */
