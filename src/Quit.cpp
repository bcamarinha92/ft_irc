#include "../inc/ft_irc.hpp"

void cmdQuit(Server &irc, Message *message, int sender)
{
    std::string reason = parseRealname(message->get_buffer());
    Client &client = irc.getClientByFd(sender);
    disconnectClient(irc, client, reason);
}
