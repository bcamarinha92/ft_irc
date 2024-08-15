#ifndef CHANNEL_HPP
# define CHANNEL_HPP

# include "ft_irc.hpp"

class Client;
class Server;

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
		std::vector<int>		getChannelClientsFds();
		std::vector<int>		getChannelInvites() const;
		size_t					getChannelUserLimit() const;
		std::string				getChannelKey() const;

		//Setters
		void					setName(std::string name);
		void					setTopic(std::string topic);
		void					switchLaunch();
		void					setChannelUserLimit(size_t limit);
		void					setChannelKey(std::string key);

		//Methods
		void					prepareModes();
		void					addClient(const Client& client, Server &irc);;
		void					rmClient(int clientSocket, Server &irc);
		void					addOperator(const Client& client);
		void					rmOperator(int clientSocket);
		bool					activateMode(char mode, int sender, bool join);
		bool					deactivateMode(char mode, int sender);
		bool					checkOperatorRole(int fd);
		bool					checkChannelMode(char mode);
		void					addInvite(int fd);
		void					rmInvite(int fd);
		bool					checkChannelUserInvite(int fd);

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
		ssize_t					_ulimit;
		std::string				_key;
		std::vector<int>		_invites;
};

std::ostream&			operator<<(std::ostream& o, Channel const& i);

#endif /* ********************************************************* CHANNEL_H */
