CC = c++
NAME = web_server
CPPFLAGS = -Wall -Wextra -Werror -fsanitize=address -g3
USER = $(shell whoami)
OBJ = $(patsubst %.cpp,%.o,$(wildcard *$(USER).cpp))

all: $(NAME)
$(NAME): $(OBJ)
	$(CC) $(CPPFLAGS) $(OBJ) -o $(NAME)

%.o:%.cpp
	$(CC) $(CPPFLAGS) -c -o $@ $^

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean all	

.PHONY: clean fclean all