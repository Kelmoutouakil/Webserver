cc = c++
NAME = web_server
CPPFLAGS = -Wall -Wextra -Werror -fsanitize=address -g3
OBJ = $(patsubst %.cpp,%.o,$(wildcard *a.cpp))

all: $(NAME)
$(NAME): $(OBJ) $(HEADER)
	$(CC) $(CPPFLAGS) $(OBJ) -o $(NAME)

%.o:%.cpp
	$(CC) $(CPPFLAGS) -c -o $@ $^

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean all	

.PHONY: clean fclean all