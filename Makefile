cc = c++
NAME = web_server
CPPFLAGS = -Wall -Wextra -Werror -fsanitize=address
OBJ = $(patsubst %.c,%.o,$(wildcard *.cpp))

all: $(NAME)

$(NAME): $(OBJ)
	c++ $(CPPFLAGS) $^ -o $@

clean:
	rm -rf $(OBJ)

fclean: clean
	rm -rf $(NAME)
re: fclean all