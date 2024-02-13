SRCS = ft_isalnum.c ft_isalpha.c ft_isdigit.c ft_isascii.c ft_isprint.c \
		ft_strchr.c ft_strlcat.c ft_strlen.c ft_strncmp.c ft_strnstr.c \
		ft_strrchr.c ft_tolower.c ft_toupper.c ft_strlcpy.c ft_atoi.c \
		ft_memset.c ft_memcpy.c ft_bzero.c ft_memchr.c ft_memcmp.c \
		ft_memmove.c ft_calloc.c ft_strdup.c ft_substr.c ft_strjoin.c \
		ft_putchar_fd.c ft_putstr_fd.c ft_putendl_fd.c ft_putnbr_fd.c \
		ft_striteri.c ft_strmapi.c ft_strtrim.c ft_split.c ft_itoa.c \
		ft_printf.c ft_puthex.c ft_printf_utils.c ft_printf_adress.c \
		ft_lstnew.c ft_lstadd_front.c ft_lstsize.c ft_lstlast.c \
		ft_lstadd_back.c ft_lstdelone.c ft_lstclear.c ft_lstiter.c \
		ft_lstmap.c ft_get_next_line.c ft_get_next_line_utils.c

OBJS = $(SRCS:.c=.o)
NAME = libft.a
CC = gcc
ARFLAGS = -rcs
CFLAGS = -Wall -Wextra -Werror -g

all: $(NAME)

$(NAME):
	@$(CC) $(CFLAGS) -c $(SRCS) $(BONUSSRCS)
	@ar $(ARFLAGS) $(NAME) $(OBJS)

clean:
	@$(RM) $(OBJS) $(BONUSOBJS)

fclean: clean
	@$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re