# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: brumarti <brumarti@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/10/16 20:47:33 by bde-sous          #+#    #+#              #
#    Updated: 2024/07/04 19:24:24 by brumarti         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CXX = c++
FLAGS = -Wall -Wextra -Werror -std=c++98 #-fsanitize=address
FILES = ./src/main.cpp ./src/Client.cpp ./src/Message.cpp #./src/Channel.cpp  ./src/Server.cpp
OBJS = $(FILES:.cpp=.o)
NAME = ft_irc 
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