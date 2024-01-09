cc = c++
NAME = web_server
CPPFLAGS = -Wall -Wextra -Werror -fsanitize=address -g3
OBJ = $(patsubst %.cpp,%.o,$(wildcard *.cpp))

all: $(NAME)

$(NAME): $(OBJ)
	c++ $(CPPFLAGS) $^ -o $@

clean:
	rm -rf $(OBJ)

fclean: clean
	rm -rf $(NAME)
re: fclean all