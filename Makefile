NAME = minishell
GCC = gcc
CFLAGS = -Wall -Wextra -Werror -g
LIBFT = -Llibft -lft
SRC = minishell.c ./path/paths.c ./parser/parser.c ./parser/token_functions.c ./garbage/garbage.c utils1.c utils2.c utils3.c ./free/free_list.c map/map.c exec.c
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