CC = c++
NAME = webserv
CPPFLAGS = -Wall -Wextra -Werror -std=c++98 -fsanitize=address -g
HEADER =$(wildcard *.hpp)
OBJF = obj
OBJ = $(patsubst %.cpp, $(OBJF)/%.o, $(wildcard *.cpp))

all: $(NAME)

$(NAME): $(OBJF) $(OBJ) $(HEADER) 
	$(CC) $(CPPFLAGS) $(OBJ) -o $(NAME)

#%.o:%.cpp
$(OBJF)/%.o:%.cpp
	$(CC) $(CPPFLAGS) -c -o $@ $<
$(OBJF):
	mkdir $(OBJF)

clean:
	rm -f $(OBJF)

fclean: clean
	rm -f $(NAME)

re: fclean all	

.PHONY: clean fclean all