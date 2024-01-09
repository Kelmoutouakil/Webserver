cc = c++
NAME = web_server
CPPFLAGS = -Wall -Wextra -Werror -fsanitize=address
OBJ = $(patsubst %.cpp,%.o,$(wildcard *.cpp))

echo:
	echo $(OBJ)
all: $(NAME)

$(NAME): $(OBJ)
	c++ $(CPPFLAGS) $^ -o $@

clean:
	rm -rf $(OBJ)

fclean: clean
	rm -rf $(NAME)
re: fclean all