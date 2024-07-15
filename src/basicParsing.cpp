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

std::string getChannelFromBuffer(const std::string &input)
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
		param.push_back(buffer.substr(start, i - start));
		if (buffer[i] && buffer[i] != ' ' && buffer[i] != ':' && buffer[i] != '\n' && buffer[i] != '\r')
		{
			i++;
		}
	}

	return param;
}
