# include "../../inc/BotClient.hpp"

std::vector<std::string>	split(const std::string &s, char delim)
{
    std::vector<std::string>	elems;
    std::stringstream			ss(s);
    std::string					item;

    while (std::getline(ss, item, delim))
        elems.push_back(item);
    return elems;
}

void	get_bot_command(std::string buffer, BotClient& b)
{
	std::vector<std::string>	elems = split(buffer, ' ');

	for (size_t i = 0; i < elems.size(); i++)
	{
	  	if (elems[i] == "PING")
			return cmdPing(elems, i, b);
	  	else if (elems[i] == "PRIVMSG")
			return cmdPrivMsg(elems, i, buffer, b);
	  	else if (elems[i] == "INVITE")
			return cmdInvite(elems, i, b);

	}
}

void	initializeRandomSeed()
{
    std::srand(static_cast<unsigned int>(std::time(0)));
}

int	main(int argc, char **argv)
{
	if (argc != 4)
		return 1;

	try
	{
		char	buffer[512];

		initializeRandomSeed();
		BotClient	bot(std::string(argv[1]), atoi(argv[2]), std::string(argv[3]));

		std::string passCmd = "PASS " + bot.getPassword() + "\r\n";
    	send(bot.getSocket(), passCmd.c_str(), passCmd.length(), 0);

    	// Send nick and user commands
    	std::string nickCmd = "NICK " + bot.getNickname() + "\r\n";
		send(bot.getSocket(), nickCmd.c_str(), nickCmd.length(), 0);
    	std::string userCmd = "USER " + bot.getUsername() + " " + bot.getNickname() + " bla :" + bot.getRealname() + "\r\n";
		send(bot.getSocket(), userCmd.c_str(), userCmd.length(), 0);

		// Join channel
    	//std::string joinCmd = "JOIN " + std::string(IRC_CHANNEL) + "\r\n";
    	//send(bot.getSocket(), joinCmd.c_str(), joinCmd.length(), 0);

    	// Send message
    	//std::string msgCmd = "PRIVMSG " + std::string(IRC_CHANNEL) + " :Hey, I am your Botuxo!\r\n";
    	//send(bot.getSocket(), msgCmd.c_str(), msgCmd.length(), 0);

		// Receive and print messages
    	while (true)
		{
			recv(bot.getSocket(), buffer, 256, 0);
			std::cout << buffer << std::endl;
			get_bot_command(buffer, bot);
			std::memset(buffer, 0, sizeof(buffer));
    	}
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << '\n';
	}
    return 0;
}
