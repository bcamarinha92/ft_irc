#ifndef CLIENT_HPP
# define CLIENT_HPP

#include "Server.hpp"
# include "ft_irc.hpp"
#include <string>

class Channel;
class Server;
class Client
{

	public:
		//Constructors
		//Constructors
		Client();
		Client(int socket);
		Client(Client const& src);

		//Destructors

		//Destructors
		~Client();

		//Overloads
		Client&							operator=(Client const& rhs);

		//Getters
		int 							getSocket() const;
		int 							getPingCount() const;
		std::string 					getNickname() const;
		std::string 					getRealname() const;
		std::string 					getUsername() const;
		sockaddr_in						getclientAddr() const;
		socklen_t						getclientAddrLen() const;
		std::time_t						getLastAction() const;
		std::string 					getHostname() const;
		bool                            getPwdStatus() const;
		std::vector<int>                getUserChannelsFds(Server irc) const;
		bool							getAuthenticated() const;

		//Setters
		void 							setSocket(int socket);
		void 							setNickname(const std::string& nickname);
		void 							setUsername(const std::string& username);
		void 							setRealname(const std::string& realname);
		void            				setLastAction();
		void            				setPwdStatus();
		void							setAuthenticated();
		void            				incPingCount();
		void            				resetPingCount();

		//Methods
		void							addChannel(const Channel& channel);
		void							rmChannel(std::string channelName);

		//Attributes
		pollfd 							clientPollfd;
		std::map<std::string, Channel>	channels;

	private:
		//Attributes
		std::string						_nickname;
    	std::string 					_username;
		std::string 					_realname;
		bool							_authenticated;
		sockaddr_in						_clientAddr;
		socklen_t	 					_clientAddrLen;
		int				 				_clientSocket;
		std::string						_client_ip;
		//hostent*						_hostname;
		std::string						_hostname;
		std::time_t						_lastAction;
		id_t							_pingCount;
		bool                            _pwdStatus;
};

std::ostream&			operator<<(std::ostream& o, Client const& i);

#endif /* ********************************************************** CLIENT_H */
