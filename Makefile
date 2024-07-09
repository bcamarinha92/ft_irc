# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: brumarti <brumarti@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/10/16 20:47:33 by bde-sous          #+#    #+#              #
#    Updated: 2024/07/10 18:54:40 by brumarti         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CXX = c++
FLAGS = -Wall -Wextra -Werror -std=c++98 #-fsanitize=address
FILES = ./src/main.cpp ./src/Client.cpp ./src/Message.cpp ./src/Server.cpp ./src/Channel.cpp \
		./src/gnl/get_next_line_utils_bonus.cpp ./src/gnl/get_next_line_bonus.cpp \
		./src/basicParsing.cpp # 
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