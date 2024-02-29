/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: burkaya <burkaya@student.42istanbul.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/14 22:59:11 by egumus            #+#    #+#             */
/*   Updated: 2024/02/29 22:04:42 by burkaya          ###   ########.fr       */
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

#define QUOTE_NONE 0
#define QUOTE_ONE 39
#define QUOTE_TWO 34

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
} t_token;

typedef struct s_garbage
{
	void				*ptr;
	struct s_garbage	*next;
} t_garbage;

typedef struct s_lexer
{
	char	**sp;
	int		i;
	int		is_happend;
	int		quote;
	int		is_pipe_added;
	int		take_it;
} t_lexer;

typedef struct s_exec
{
    char    *cmd_path;
    char    **cmd_args;
} t_exec;

typedef struct s_state
{
	char		*cmd;
	char		*cwd;
	char		**env;
	int			status;
	int			fd[2];
	pid_t		pid;
	t_token		*tokens;
	t_garbage	*garbage;
} t_state;

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

/* GARBAGE */
void	ft_add_garbage(t_state *s, void *ptr);
void	ft_free_garbage(t_state *s);
void	ft_addarr_garbage(t_state *s, void **ptr);

/* LEXER */
int		ft_lexer(t_state *s);
void	ft_free_tokens(t_token *token);

/* SHELL */
void	ft_start(t_state *s);

/* EXEC */
int    ft_execuator(t_state *s);

#endif