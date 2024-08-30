# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: brumarti <brumarti@student.42.fr>          +#+  +:+       +#+         #
#    By: brumarti <brumarti@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/10/16 20:47:33 by bde-sous          #+#    #+#              #
#    Updated: 2024/07/10 18:54:40 by brumarti         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CXX = c++
FLAGS = -Wall -Wextra -Werror -std=c++98 -g #-fsanitize=address
FILES = ./src/main.cpp ./src/Client.cpp ./src/Server.cpp ./src/Channel.cpp \
		./src/Parsing.cpp ./src/Who.cpp ./src/Message.cpp ./src/Utilities.cpp \
		./src/Send.cpp ./src/Invite.cpp ./src/Join.cpp ./src/Kick.cpp ./src/Mode.cpp \
		./src/Part.cpp ./src/Privmsg.cpp ./src/Cap.cpp ./src/PingPong.cpp \
		./src/Topic.cpp ./src/NameReply.cpp ./src/Pass.cpp ./src/Nick.cpp \
		./src/User.cpp ./src/Notice.cpp


OBJS = $(FILES:.cpp=.o)
NAME = ircserv
SRC_FOLDER = ./src/

all : $(NAME)

$(NAME): $(OBJS)
	$(CXX) $(FLAGS) $(FILES) -o $(NAME)

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

run: all clean
	./$(NAME)

re: fclean all
