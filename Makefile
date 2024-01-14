CC = c++
NAME = webserv
CPPFLAGS = -Wall -Wextra -Werror -std=c++98 -fsanitize=address -g
SRC = $(wildcard *.cpp)
HEADER =$(wildcard *.hpp)
OBJF = obj
OBJ = $(SRC:.cpp=.o)

all: $(NAME)
$(NAME): $(OBJ) $(HEADER)
	$(CC) $(CPPFLAGS) $(OBJ) -o $(NAME)

#%.o:%.cpp
$(OBJF)/%.o:$(OBJF)/%.cpp
	$(CC) $(CPPFLAGS) -c -o $@ $^
$(OBJF):
	mkdir $(OBJF)

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean all	

.PHONY: clean fclean all