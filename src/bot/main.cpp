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
	if (buffer.find(":Nickname is already set") != std::string::npos && b.nn < 4)
	{
		b.nn++;
		cmdNick(b);
		cmdUser(b);
	}
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
	{
		std::cerr << "Usage: " << argv[0] << " <IP> <port> <password>" << std::endl;
		return 1;
	}
	try
	{
		char	buffer[512];

		initializeRandomSeed();
		BotClient	bot(std::string(argv[1]), atoi(argv[2]), std::string(argv[3]));

		cmdPass(bot);
		cmdNick(bot);
		cmdUser(bot);

    	while (true)
		{
			if(recv(bot.getSocket(), buffer, 512, 0) <= 0)
			{
				std::cout << "Server has stopped running. Closing bot..." << std::endl;
            	close(bot.getSocket());
				break;
			}
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
