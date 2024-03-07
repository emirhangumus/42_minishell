NAME = minishell
CC = gcc
CFLAGS = -g  -Wall -Wextra -Werror -fsanitize=address
M_SRC = minishell.c env_check.c signals.c garbage.c start.c lexer.c lexer_bychar.c lexer_merge.c builtins.c helpers.c execuator.c exec_utils.c exec_init.c tokens.c debug.c
LIB_SRC = lib/ft_itoa.c lib/ft_isfullof.c lib/ft_strchr.c lib/ft_isdigit.c lib/ft_atoi.c lib/ft_arr_include.c lib/ft_arr_push.c lib/ft_arr_dup.c lib/ft_arr_len.c lib/ft_quote_split.c lib/ft_strdup.c lib/ft_strlen.c lib/ft_split.c lib/ft_strjoin.c lib/ft_strcmp.c lib/ft_strtrim.c lib/ft_substr.c lib/ft_strncmp.c lib/ft_trim_quotes.c lib/ft_free_tab.c lib/ft_isempty.c
SRC = $(M_SRC) $(LIB_SRC)
OBJ = $(SRC:.c=.o)
READLINE = -L./lib/readline/lib -I./lib/readline/include/readline -lreadline 
DIR	= $(shell echo $(PWD))
RM = rm -rf
RL = ./lib/readline/lib/libreadline.a

all: $(NAME)

$(NAME): $(OBJ) $(RL)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJ) $(READLINE)

$(RL):
	@echo "Downloading readline"
	@curl -O https://ftp.gnu.org/gnu/readline/readline-8.2-rc1.tar.gz
	@tar -xvf readline-8.2-rc1.tar.gz
	@$(RM) readline-8.2-rc1.tar.gz
	@cd readline-8.2-rc1 && ./configure --prefix=$(DIR)/lib/readline && make && make install
	@$(RM) readline-8.2-rc1
	@  echo "Readline installed"

RLclean:
	@$(RM) lib/readline
	@echo "Readline removed"

clean:
	$(RM) $(OBJ)

fclean: clean
	$(RM) $(NAME)

re: fclean all

push: fclean
	git add .
	git commit -m "$(m)"
	git push

.PHONY: all clean fclean re push