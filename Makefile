NAME = minishell
CC = gcc
CFLAGS = -g  -Wall -Wextra -Werror -fsanitize=address
M_SRC = minishell.c garbage.c start.c lexer.c lexer_bychar.c lexer_merge.c builtins.c helpers.c execuator.c exec_utils.c tokens.c debug.c
LIB_SRC = lib/ft_strchr.c lib/ft_isdigit.c lib/ft_atoi.c lib/ft_arr_include.c lib/ft_arr_push.c lib/ft_arr_dup.c lib/ft_arr_len.c lib/ft_quote_split.c lib/ft_strdup.c lib/ft_strlen.c lib/ft_split.c lib/ft_strjoin.c lib/ft_strcmp.c lib/ft_strtrim.c lib/ft_substr.c lib/ft_strncmp.c lib/ft_trim_quotes.c lib/ft_free_tab.c lib/ft_isempty.c
SRC = $(M_SRC) $(LIB_SRC)
OBJ = $(SRC:.c=.o)
READLINE = -L/usr/local/lib -I/usr/local/include -lreadline

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJ) $(READLINE)

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean all

push:
	git add .
	git commit -m "$(m)"
	git push

.PHONY: all clean fclean re push action
