#include "../inc/ft_irc.hpp"

void closeFDs(Server &irc)
{
    size_t  i;

    i = 0;
    while (i < irc.pollfds.size())
    {
        if (irc.pollfds[i].fd != irc.getServerSocket())
            irc.rmClient(irc.pollfds[i].fd, i);
        i++;
    }
    close(irc.getServerSocket());
    irc.pollfds.erase(irc.pollfds.begin());
}

void sigHandler(int signal)
{
    if (signal == SIGINT)
        running = false;
}

void setNonBlocking(int socket)
{
    fcntl(socket, F_SETFL, O_NONBLOCK);
}

std::string	toUpper(const std::string& str)
{
	std::string	res = str;
	for (size_t i = 0; i < res.length(); i++)
		res[i] = toupper(res[i]);
	return res;
}

int		aux(std::string targets)
{
	int	n = 1, i = 0;

	while (targets.find(",", i) != std::string::npos)
	{
		n++;
		i += targets.find(",", i) + 1;
	}
	return n;
}


std::vector<std::string> split(const std::string &s, char delim)
{
    std::vector<std::string>	elems;
    std::stringstream			ss(s);
    std::string					item;

    while (std::getline(ss, item, delim))
        elems.push_back(item);
    return elems;
}

std::string ft_read(int fd, ssize_t *bytesReceived)
{
    std::string	result;
    char		buffer[BUFFER_SIZE + 1];
    std::string	temp;

    *bytesReceived = recv(fd, buffer, BUFFER_SIZE, 0);
    if (*bytesReceived <= 0)
        return "";
    buffer[*bytesReceived] = '\0';
    result = buffer;
    return result;
}

int checkEndLine(std::string& str, std::string& ret)
{
    size_t	newlinePos = str.find('\n');

    if (newlinePos != std::string::npos) {
        ret = str.substr(0, newlinePos + 1);
        str = str.substr(newlinePos + 1);
        return ret.size();
    }
    return 0;
}

int get_next_linepp(int fd, std::string& ret, int flag) {
    static std::string pos[1024]; // assuming MAX_FD is 1024
    ssize_t bytesReceived;
    if (flag) {
        pos[fd].clear();
        return 0;
    }
    if (fd < 0 || BUFFER_SIZE <= 0) {
        return -1;
    }
    if (checkEndLine(pos[fd], ret))
        return 1;
    pos[fd] += ft_read(fd, &bytesReceived);
    if (bytesReceived == 0)
        return 0;
    checkEndLine(pos[fd], ret);
    return 1;
}
