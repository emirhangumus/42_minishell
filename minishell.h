/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: egumus <egumus@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/14 22:59:11 by egumus            #+#    #+#             */
/*   Updated: 2024/03/01 14:53:41 by egumus           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <string.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <readline/history.h>
# include <signal.h>
# include <stdarg.h>
# include <fcntl.h>

# define T_CMD 1
# define T_ARG 2
# define T_PIPE 3

# define QUOTE_NONE 0
# define QUOTE_ONE 39
# define QUOTE_TWO 34

# define CMD_PATH 19
# define CMD_BUILTIN 20
# define CMD_INPUT 21
# define CMD_OUTPUT 22

# define COLOR_RED "\x1b[31m"
# define COLOR_GREEN "\x1b[32m"
# define COLOR_YELLOW "\x1b[33m"
# define COLOR_BLUE "\x1b[34m"
# define COLOR_MAGENTA "\x1b[35m"
# define COLOR_CYAN "\x1b[36m"
# define COLOR_RESET "\x1b[0m"

# define ERR_CMD_NOT_FOUND 127
# define ERR_PIPE_INIT 126

typedef struct s_token
{
	int				type;
	char			*value;
	struct s_token	*next;
}	t_token;

typedef struct s_garbage
{
	void				*ptr;
	struct s_garbage	*next;
}	t_garbage;

typedef struct s_lexer
{
	char	**sp;
	char	*str;
	int		i;
	int		is_happend;
	int		quote;
	int		is_pipe_added;
	int		take_it;
}	t_lexer;

typedef struct s_exec
{
	char	*cmd_path;
	char	**cmd_args;
	int		type;
}	t_exec;

typedef struct s_state
{
	char		*cmd;
	char		*cwd;
	char		**env;
	int			status;
	int			fd;
	pid_t		pid;
	t_token		*tokens;
	t_garbage	*garbage;
}	t_state;

/* LIB */
size_t	ft_strlen(const char *s);
char	*ft_strdup(char *str, t_state *s);
char	**ft_split(char *s, char c, t_state *state);
char	*ft_strjoin(char const *s1, char const *s2, t_state *s);
int		ft_strcmp(const char *s1, const char *s2);
char	*ft_strtrim(char const *s1, char const *set, t_state *s);
char	*ft_substr(const char *s, unsigned int start, size_t len, t_state *st);
char	*ft_strchr(const char *s, int c);
char	**ft_quote_split(char *s, char c, t_state *state);
int		ft_strncmp(const char *s1, const char *s2, size_t n);
char	*ft_trim_quotes(char const *str, t_state *s);
void	ft_free_tab(char **tab);
int		ft_is_empty(char *s);

/* GARBAGE */
void	ft_add_garbage(t_state *s, void *ptr);
void	ft_free_garbage(t_state *s);
void	ft_addarr_garbage(t_state *s, void **ptr);

/* LEXER */
int		ft_lexer(t_state *s);
void	ft_free_tokens(t_token *token);
int		ft_toggle_quote(t_lexer *l, char c);
int		ft_merge_args_init(t_token *start_token, int *ta, t_token **wc);
int		ft_merge_args_iterate(t_token **tmp, t_state *s, char **str);
void	ft_merge_args(t_token *start_token, t_state *s);
int		ft_take_it(t_lexer *l, t_state *s, int *i, int *j);
void	ft_lexer_bychar_pipe(t_state *s, t_lexer *l, int *i, int *j);
int		ft_lexer_bychar_iterate(t_state *s, t_lexer *l, int	*i, int	*j);
int		ft_lexer_bychar(t_state *s, t_lexer *l);

/* SHELL */
void	ft_start(t_state *s);

/* EXEC */
int		ft_execuator(t_state *s);
int		ft_amount_cmd(t_token *tokens);
int		ft_find_arg_amount(t_token *tokens);
char	*ft_get_cmd_path(t_token *start_token, t_state *s);

/* BUILTIN */
int		ft_is_builtin(char *value);
int		ft_execute_builtin(t_exec *exec, t_state *s, int pipefd[2]);

/* HELPERS */
char	*ft_get_env(char **env, char *key);

/* TOKENS */
void	ft_add_token(t_state *s, char *token, int type);
void	ft_create_token(t_token **token, char *value, int type);
t_token	*ft_get_last_token(t_token *token);
void	ft_remove_tokens(t_token **token, int (*f)(void *));
void	ft_free_tokens(t_token *token);

/* DEBUG */
void	ft_print_tokens(t_token *token);

#endif