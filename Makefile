CC = c++
NAME = webserv
CPPFLAGS = -Wall -Wextra -Werror -std=c++98 -fsanitize=address
HEADER =$(wildcard *.hpp)
OBJF = obj
OBJ = $(patsubst %.cpp, $(OBJF)/%.o, $(wildcard *.cpp))

all: $(NAME)

$(NAME): $(OBJF) $(OBJ) $(HEADER) 
	$(CC) $(CPPFLAGS) $(OBJ) -o $(NAME)

$(OBJF)/%.o:%.cpp
	$(CC) $(CPPFLAGS) -c -o $@ $<
$(OBJF):
	@mkdir $(OBJF)

clean:
	@rm -rf $(OBJF)

fclean: clean
	@rm -rf $(NAME)

re: fclean all	

.PHONY: clean fclean all