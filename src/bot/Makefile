CXX = c++
FLAGS = -Wall -Wextra -Werror -std=c++98 -g #-fsanitize=address
FILES = ./main.cpp ./BotCmds.cpp ./BotClient.cpp


OBJS = $(FILES:.cpp=.o)
NAME = bot

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
