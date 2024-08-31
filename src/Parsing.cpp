#include "../inc/ft_irc.hpp"

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
        if (std::isprint(*it) || *it == 1)
            cleanedStr += *it;
    }
    return cleanedStr;
}

std::string	getChannelFromBuffer(const std::string& input)
{
	std::string::size_type	start = 0, end = 0;
	std::string				cmd = cleanString(get_buffer_command(input));
	std::string				comandos[9] =
	{
		"PRIVMSG",
		"JOIN",
		"INVITE",
		"WHO",
		"MODE",
		"PART",
		"INVITE",
		"KICK",
		"NOTICE"
	};

	for (int i = 0; i < 9; i++)
	{
		if (toUpper(cmd) == comandos[i])
		{
			std::string	uInput = toUpper(input);
			start = uInput.find(comandos[i]);
			start += comandos[i].length();
		}
	}
	if (start == std::string::npos)
        return "";
	end = start;
	if (cmd == "PRIVMSG" || cmd == "NOTICE")
	{
		start++;
		end = start + input.substr(start).find_first_of((": \r\n"));
	}
	else if (input.find("#", start) != std::string::npos)
	{
		start = input.find("#", start);
		end = input.find(" ", start);
		if (end == std::string::npos)
		{
			end = input.find("\r", start);
			if (end == std::string::npos)
			{
				end = input.find("\n", start);
				if (end == std::string::npos)
					return "";
			}
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
	if (start == i)
		return "";
	return toUpper(cleanString((buffer.substr(start, i))));
}

size_t	clearBufferStr(const std::string &buffer)
{
	std::string	str = buffer;
	size_t		i = 0;

	if (buffer[i] == ':')
	{
		while (buffer[i] && buffer[i] != ' ')
			i++;
		i++;
		str = buffer.substr(0, i);
	}
	i += cleanString(get_buffer_command(buffer)).length();
	while (buffer[i] == ' ')
		i++;
	if (buffer[i] =='\r' || buffer[i] == '\n')
		return -1;
	str = buffer.substr(i, str.length() - i);
	i += cleanString(getChannelFromBuffer(buffer)).length();
	while (buffer[i] == ' ')
		i++;
	if (buffer[i] =='\r' || buffer[i] == '\n')
		return -1;
	str = buffer.substr(i, buffer.length() - i);
	while (buffer[i] == ' ')
		i++;
	if (buffer[i] =='\r' || buffer[i] == '\n')
		return -1;
	return i;
}

std::vector<std::string> get_buffer_parameters(const std::string &buffer)
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
	while (buffer[i])
	{
		if (buffer[i] == ':' && (cmd == "PART" || cmd == "PRIVMSG" || cmd == "KICK" || cmd == "NOTICE"))
		{
			i++;
			size_t start = i;
			while (buffer[i] && buffer[i] != '\n' && buffer[i] != '\r')
				i++;
			if (start != i)
				param.push_back(cleanString(buffer.substr(start, i - start)));
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

std::string	parseRealname(const std::string& input)
{
    size_t colonPos = input.find(':');

    if (colonPos != std::string::npos)
        return input.substr(colonPos + 1);
    return "";
}

std::string	removeSpaces(const std::string& str)
{
	std::string	res;
	size_t		i = 0;
	bool		foundColon = false;

	while (i < str.size() && str[i] == ' ')
        i++;
	for (; i < str.size(); i++)
	{
		if (str[i] == ':')
		{
			foundColon = true;
			res += str[i];
		}
		else if (str[i] == ' ')
		{
	  		if (foundColon)
				res += str[i];
	  		else if (!res.empty() && res[res.size() - 1] != ' ')
				res += ' ';
		}
		else
	  		res += str[i];
	}
	return res;
}

bool valid_nick(const std::string &nick, Server &irc, int sender)
{
	if (nick.length() > irc.max_len) {
		sendMessage(sender, ERR_ERRONEUSNICKNAME(irc.getHostname(), nick), ERR432);
		return false;
	}
	if (nick[0] == '#' || nick[0] == ':') {
		sendMessage(sender, ERR_ERRONEUSNICKNAME(irc.getHostname(), nick), ERR432);
		return false;
	}

	for (size_t i = 0; i < nick.length(); i++)
	{
		if (!std::isalnum(nick[i]) && nick[i] != '[' && nick[i] != ']' && nick[i] != '\\' && nick[i] != '{' && nick[i] != '}' && nick[i] != '|' && nick[i] != '_' && nick[i] != '-')
		{
			sendMessage(sender, ERR_ERRONEUSNICKNAME(irc.getHostname(), nick), ERR432);
			return false;
		}
	}
	return true;
}

bool valid_channel(const std::string &channel, Server &irc)
{
	if (channel.length() > irc.max_len) {
		return false;
	}
	for (size_t i = 0; i < channel.size(); i++)
	{
		if (channel[i] == ' ' || channel[i] == 0x07 || channel[i] == ',')
			return false;
	}

	return true;
}
