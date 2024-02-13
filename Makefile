NAME = ms
GCC = gcc
CFLAGS = -Wall -g
LIBFT = -Llibft -lft
SRC = main.c
OBJ = $(SRC:.c=.o)
READLINE = -L/usr/local/lib -I/usr/local/include -lreadline

all: libft $(NAME)

libft:
	make -C ./libft

$(NAME): $(OBJ)
	$(GCC) $(CFLAGS) -o $(NAME) $(OBJ) $(LIBFT) $(READLINE)

clean:
	rm -f $(OBJ)
	make -C libft clean

fclean: clean
	rm -f $(NAME)
	make -C libft fclean

re: fclean all

.PHONY: all clean fclean re libft