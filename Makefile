NAME = minishell
GCC = gcc
CFLAGS = -Wall -Wextra -Werror -g
SRC = minishell.c garbage.c start.c lexer.c lib/ft_quote_split.c lib/ft_strdup.c lib/ft_strlen.c lib/ft_split.c lib/ft_strjoin.c lib/ft_strcmp.c lib/ft_strtrim.c lib/ft_substr.c lib/utils.c lib/ft_strncmp.c lib/ft_trim_quotes.c execuator.c
OBJ = $(SRC:.c=.o)
READLINE = -L/usr/local/lib -I/usr/local/include -lreadline

all: $(NAME)

$(NAME): $(OBJ)
	$(GCC) $(CFLAGS) -o $(NAME) $(OBJ) $(READLINE)

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re