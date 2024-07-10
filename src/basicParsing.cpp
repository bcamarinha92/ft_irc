#include "../inc/Server.hpp"

std::string getNickFromBuffer(const std::string& input) 
{
    std::istringstream iss(input);
    std::string line;
    
    while (std::getline(iss, line)) 
    {
        if (line.substr(0, 5) == "NICK ")
            return line.substr(5);
    }
    return "";
}

std::string getChannelFromBuffer(const std::string& input) 
{
    std::istringstream iss(input);
    std::string line;
    
    while (std::getline(iss, line)) 
    {
        if (line.substr(0, 5) == "JOIN ")
            return line.substr(5);
    }
    return "";
}

std::string get_buffer_command(const std::string buffer) {
	int i = 0;
	int start = 0;

	if (buffer[i] == ':') {
		while (buffer[i] != ' ') {
			i++;
		}
		i++;
	}

	start = i;
	while(buffer[i] != ' ') {
		i++;
	}

	return (buffer.substr(start, i));
}

std::vector<std::string> get_buffer_parameters(const std::string buffer) {
	int i = 0;
	int start = 0;
	std::vector<std::string> param;

	if (buffer[i] == ':') {
		while (buffer[i] != ' ') {
			i++;
		}
		i++;
	}
	while(buffer[i] != ' ') {
		i++;
	}
	i++;

	while (1) {
		start = i;
		while (buffer[i] && buffer[i] != ' ')
			i++;
		param.push_back(buffer.substr(start, i - 1));
		if (!buffer[i])
			break;
		i++;
	}

	return (param);
}