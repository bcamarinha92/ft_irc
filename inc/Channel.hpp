#ifndef CHANNEL_HPP
# define CHANNEL_HPP

# include "ft_irc.hpp"
# include <ctime>

class Client;

class Channel
{
	public:
		//Constructors
		Channel();
		Channel(std::string name);
		Channel(Channel const& src);

		//Destructors
		~Channel();

		//Overloads
		Channel&				operator=(Channel const& rhs);

		//Getters
		std::string				getName() const;
		std::string				getTopic() const;
		std::time_t				getCreatedAtTime() const;
		bool					getLaunch() const;
		std::string				getChannelModes();
		std::map<int, Client>	getChannelClients(bool op);
		std::string				getLocalIPAddress();
		std::vector<int>		getChannelClientsFds();

		//Setters
		void					setName(std::string name);
		void					setTopic(std::string topic);
		void					switchLaunch();

		//Methods
		void					addClient(const Client& client);
		void					rmClient(int clientSocket);
		void					addOperator(const Client& client);
		void					rmOperator(int clientSocket);
		void					prepareModes();
		bool					activateMode(char mode, int sender, bool join);
		bool					deactivateMode(char mode, int sender);
		bool					checkOperatorRole(int fd);
		void					sendMsgToChannelClients(std::string msg);

		//Attributes
		std::map<int, Client>	members;
		std::map<int, Client>	operators;

	private:
		//Attributes
		std::string				_name;
		std::string				_topic;
		std::map<char, bool>	_modes;
		std::time_t				_createdAt;
		bool					_launch;
};

std::ostream&			operator<<(std::ostream& o, Channel const& i);

#endif /* ********************************************************* CHANNEL_H */
