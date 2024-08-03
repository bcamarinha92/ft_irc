#include "../inc/Server.hpp"

std::string	getNickFromBuffer(const std::string &input)
{
	std::istringstream	iss(input);
	std::string			line;

	while (std::getline(iss, line))
	{
		if (line.substr(0, 5) == "NICK ")
			return cleanString(line.substr(5));
	}
	return "";
}

std::string	cleanString(const std::string& name)
{
    std::string	cleanedStr;

    for (std::string::const_iterator it = name.begin(); it != name.end(); ++it)
	{
        if (std::isprint(*it))
            cleanedStr += *it;
    }
    return cleanedStr;
}

std::string	getChannelFromBuffer(const std::string& input)
{
	int						i = 0;
	std::string::size_type	start = 0, end = 0;
	std::string				comandos[6] =
	{
		"PRIVMSG",
		"JOIN",
		"INVITE",
		"WHO",
		"MODE",
		"PART"
	};

	while (i < 6)
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
	end = start;
	if (input.find("#", start) != std::string::npos)
	{
		start = input.find("#", start);
		end = input.find(" ", start);
		if (end == std::string::npos)
		{
			end = input.find("\r", start);
			if (end == std::string::npos)
				return "";
		}
	}
	else if (start == end)
	{
		start++;
		end = (input.find(" ", start) != std::string::npos ? input.find(" ", start) : input.find('\r', start));
		if (end == std::string::npos)
			return "";
	}
    std::string::size_type begin = start;
    while (begin < end && isspace(input[begin]))
        ++begin;
    std::string::size_type finish = end - 1;
    while (finish > begin && isspace(input[finish]))
        --finish;
    std::string result = input.substr(begin, finish - begin + 1);
   	return cleanString(result);
}

std::string	getModeFromBuffer(const std::string& input)
{
	std::istringstream	iss(input);
    std::string			line;

	while (std::getline(iss, line))
    {
        if (line.substr(0, 5) == "MODE ")
        {
			std::string	chn = cleanString(getChannelFromBuffer(input));
			if ((line.substr(6 + chn.size(), 1) == "+" || line.substr(6 + chn.size(), 1) == "-") \
				&& line.substr(6 + chn.size()).size() > 1)
				return cleanString(line.substr(6 + chn.size()));
			else if (line.substr(0, 5 + chn.size()) == "MODE " + chn)
				return "\n";
		}
    }
	return "";
}

std::string	get_buffer_command(const std::string buffer)
{
	int	i = 0;
	int	start = 0;

	if (buffer[i] == ':')
	{
		while (buffer[i] != ' ')
			i++;
		i++;
	}
	start = i;
	while ((buffer[i] != ' ') && (buffer[i] != '\r') && (buffer[i] != '\n'))
		i++;
	return cleanString((buffer.substr(start, i)));
}

/*std::vector<std::string> get_buffer_parameters(const std::string &buffer)
{
	std::vector<std::string>	param;
	size_t						i = 0;
	std::string					cmd = cleanString(get_buffer_command(buffer));

	if (buffer[i] == ':')
	{
		while (buffer[i] && buffer[i] != ' ')
			i++;
		i++;
	}
	while (buffer[i] && buffer[i] != ' ' && (buffer[i] != '\r') && (buffer[i] != '\n'))
		i++;
	if (buffer[i] =='\r' || buffer[i] == '\n')
		return param;
	i++;
	if (cmd == "PRIVMSG")
	{
		while (buffer[i] && buffer[i] != ' ' && buffer[i] != '\r')
			i++;
	}
	while (buffer[i])
	{
		if (buffer[i] == ':' && (cmd == "PART" || cmd == "PRIVMSG"))
		{
			i++;
			if (cmd == "PRIVMSG")
			{
				size_t start = i;
				while (buffer[i] && buffer[i] != '\n' && buffer[i] != '\r')
					i++;
				if (start != i)
					param.push_back(cleanString(buffer.substr(start, i - start)));
			}
		}
		size_t start = i;
		while (buffer[i] && buffer[i] != ' ' && buffer[i] != ':' && buffer[i] != '\n' && buffer[i] != '\r')
			i++;
		if (start != i)
			param.push_back(cleanString(buffer.substr(start, i - start)));
		if ((buffer[i] == ':' && cmd != "PART") || buffer[i] == '\n' || buffer[i] == '\r')
			break;
		else
			i++;
	}
	std::cout << "Parameters: ";
	for (size_t i = 0; i < param.size(); i++)
		std::cout << i << ": " << param[i] << " ";
	std::cout << std::endl;
	return param;
}*/

std::vector<std::string> get_buffer_parameters(const std::string &buffer)
{
	std::string					nbuffer = buffer;
	std::vector<std::string>	param;
	size_t						i = 0;
	std::string					cmd = cleanString(get_buffer_command(buffer));

	if (buffer[i] == ':')
	{
		while (buffer[i] && buffer[i] != ' ')
			i++;
		i++;
		nbuffer = buffer.substr(0, i);
	}
	i += cleanString(get_buffer_command(buffer)).length();
	while (buffer[i] == ' ')
		i++;
	if (buffer[i] =='\r' || buffer[i] == '\n')
		return param;
	nbuffer = buffer.substr(i, nbuffer.length() - i);
	i += cleanString(getChannelFromBuffer(buffer)).length();
	while (buffer[i] == ' ')
		i++;
	if (buffer[i] =='\r' || buffer[i] == '\n')
		return param;
	nbuffer = buffer.substr(i, buffer.length() - i);
	while (buffer[i] == ' ')
		i++;
	if (buffer[i] =='\r' || buffer[i] == '\n')
		return param;
	while (buffer[i])
	{
		if (buffer[i] == ':' && (cmd == "PART" || cmd == "PRIVMSG"))
		{
			i++;
			if (cmd == "PRIVMSG")
			{
				size_t start = i;
				while (buffer[i] && buffer[i] != '\n' && buffer[i] != '\r')
					i++;
				if (start != i)
					param.push_back(cleanString(buffer.substr(start, i - start)));
			}
		}
		size_t start = i;
		while (buffer[i] && buffer[i] != ' ' && buffer[i] != ':' && buffer[i] != '\n' && buffer[i] != '\r')
			i++;
		if (start != i)
			param.push_back(cleanString(buffer.substr(start, i - start)));
		if ((buffer[i] == ':' && cmd != "PART") || buffer[i] == '\n' || buffer[i] == '\r')
			break;
		else
			i++;
	}
	return param;
}
