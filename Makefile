# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: bde-sous <bde-sous@student.42porto.com>    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/10/16 20:47:33 by bde-sous          #+#    #+#              #
#    Updated: 2023/11/20 21:42:13 by bde-sous         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CXX = c++
FLAGS = -Wall -Wextra -Werror -std=c++98 #-fsanitize=address
FILES = ./src/main.cpp ./src/Client.cpp ./src/Server.cpp ./src/Channel.cpp \
		./src/gnl/get_next_line_utils_bonus.cpp ./src/gnl/get_next_line_bonus.cpp \
		./src/basicParsing.cpp ./src/commands.cpp #./src/Message.cpp

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
