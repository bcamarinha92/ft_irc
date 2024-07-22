#include "../inc/Server.hpp"

std::string getNickFromBuffer(const std::string& input)
{
    std::istringstream	iss(input);
    std::string			line;

    while (std::getline(iss, line))
    {
        if (line.substr(0, 5) == "NICK ")
            return line.substr(5);
    }
    return "";
}

std::string getChannelFromBuffer(const std::string& input)
{
    std::istringstream	iss(input);
    std::string			line;

    while (std::getline(iss, line))
    {
        if (line.substr(0, 5) == "JOIN ")
            return line.substr(5);
		if (line.substr(0, 4) == "WHO ")
            return line.substr(4);
    }
    return "";
}

std::string	getModeFromBuffer(const std::string& input)
{
	std::istringstream	iss(input);
    std::string			line;
	while (std::getline(iss, line))
    {
        if (line.substr(0, 5) == "MODE ")
        {
			if (line.substr(6, 1) == "+" || line.substr(6, 1) == "-")
			{
				std::cout << line.substr(6) << std::endl;
				return line.substr(6);
			}
		}
		if (line.substr(0, 5) == "MODE\n")
			return "\n";
    }
	return "";
}

void printClientMap(std::map<int, Client*> clientMap)
{
    std::map<int, Client*>::const_iterator it;
    for (it = clientMap.begin(); it != clientMap.end(); ++it)
        std::cout << "Key: " << it->first << ", Nickname: " << (*it->second).getNickname() << std::endl;
}
