#include "../inc/Server.hpp"

std::string getNickFromBuffer(const std::string &input)
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
	int i;
	std::string::size_type start, end;
	std::string comandos[4] = {
		"PRIVMSG",
		"JOIN",
		"INVITE",
		"WHO"
	};

	i = 0;
	while (i < 4)
	{
		start = input.find(comandos[i]);
		if (start != std::string::npos)
		{
			start += comandos[i].length();
			break;
		}
		i++;
	}
	if (start == std::string::npos) 
        return "";
	if (comandos[i] == "PRIVMSG")
	{
		end = input.find(":", start);
		if (end == std::string::npos) 
			return "";
	}
	else
		end = input.find('\n', start);
    std::string::size_type begin = start;
    while (begin < end && isspace(input[begin]))
        ++begin;
    std::string::size_type finish = end - 1;
    while (finish > begin && isspace(input[finish]))
        --finish;
    std::string result = input.substr(begin, finish - begin + 1);
   	return result;
}


std::string get_buffer_command(const std::string buffer)
{
	int i = 0;
	int start = 0;

	if (buffer[i] == ':')
	{
		while (buffer[i] != ' ')
		{
			i++;
		}
		i++;
	}

	start = i;
	while (buffer[i] != ' ')
	{
		i++;
	}

	return (buffer.substr(start, i));
}

std::string	get_buffer_prefix(const std::string buffer) {
	int	i = 1;

	if (buffer[0] != ':')
		return "";
	while (buffer[i] && buffer[i] != ' ') {
		i++;
	}
	return (buffer.substr(1, i - 1));
}

std::vector<std::string> get_buffer_parameters(const std::string &buffer)
{
	std::vector<std::string> param;
	size_t i = 0;

	if (buffer[i] == ':')
	{
		while (buffer[i] && buffer[i] != ' ')
		{
			i++;
		}
		i++;
	}

	while (buffer[i] && buffer[i] != ' ')
	{
		i++;
	}
	i++;

	while (buffer[i])
	{
		size_t start = i;
		while (buffer[i] && buffer[i] != ' ' && buffer[i] != ':' && buffer[i] != '\n' && buffer[i] != '\r')
		{
			i++;
		}
		if (i != start) {
			param.push_back(buffer.substr(start, i - start));
		}
		if (buffer[i] == ':' || buffer[i] == '\n' || buffer[i] == '\r')
		{
			break;
		}
		else
		{
			i++;
		}
	}

	return param;
}

std::string get_buffer_trailing(const std::string buffer) {
	std::size_t index;

	if (buffer[0] == ':') {
		index = buffer.substr(1, buffer.size() - 1).find(':');  
	} else {
		index = buffer.find(':');
	}

	if (index != std::string::npos) {
		return buffer.substr(index + 1, buffer.size() - (index + 1));
	} else {
		return "";
	}
}
