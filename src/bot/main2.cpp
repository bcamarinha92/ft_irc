# include "../../inc/BotClient.hpp"

#define IRC_CHANNEL "#test"

int	main(int argc, char **argv)
{
	if (argc != 4)
		return 1;

	try
	{
		char	buffer[512];


		BotClient	bot(std::string(argv[1]), atoi(argv[2]), std::string(argv[3]));

		std::cout << "oi\n";
		std::string passCmd = "PASS " + bot.getPassword() + "\r\n";
    	send(bot.getSocket(), passCmd.c_str(), passCmd.length(), 0);
    // Send nick and user commands
    	std::string nickCmd = "NICK " + bot.getNickname() + "\r\n";
		send(bot.getSocket(), nickCmd.c_str(), nickCmd.length(), 0);
    	std::string userCmd = "USER " + bot.getUsername() + " " + bot.getNickname() + " bla :" + bot.getRealname() + "\r\n";
		send(bot.getSocket(), userCmd.c_str(), userCmd.length(), 0);

		// Join channel
    	std::string joinCmd = "JOIN " + std::string(IRC_CHANNEL) + "\r\n";
    	send(bot.getSocket(), joinCmd.c_str(), joinCmd.length(), 0);

    	// Send message
    	std::string msgCmd = "PRIVMSG " + std::string(IRC_CHANNEL) + " :Hello, world!\r\n";
    	send(bot.getSocket(), msgCmd.c_str(), msgCmd.length(), 0);

		// Receive and print messages
    	while (true)
		{
        	recv(bot.getSocket(), buffer, 256, 0);
        	std::cout << buffer << std::endl;
    	}
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << '\n';
	}
    return 0;
}
