/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: burkaya <burkaya@student.42istanbul.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/14 22:59:11 by egumus            #+#    #+#             */
/*   Updated: 2024/03/25 14:52:41 by burkaya          ###   ########.fr       */
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
# include <signal.h>
# include <stdarg.h>
# include <fcntl.h>
# include <sys/ioctl.h>
# include <sys/stat.h>
//ernoh
# include <errno.h>

// linux
# include <sys/wait.h>

# define SYS_EXIT 42

# define T_CMD 1
# define T_ARG 2
# define T_LREDIR 3
# define T_RREDIR 4
# define T_RAPPEND 5
# define T_LAPPEND 6

# define QUOTE_NONE 0
# define QUOTE_ONE 39
# define QUOTE_TWO 34

# define ENV_RECHECK -1
# define ENV_NO_MORE -2
# define ENV_SINGLE_VALUE -3
# define ENV_ERROR -4

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
# define ERR_IS_A_DIRECTORY 126
# define ERR_PERMISSION_DENIED 125
# define ERR_NUMERIC_ARG 255
# define ERR_PIPE_INIT 124
# define ERR_NOT_VALID_IDENTIFIER 123
# define ERR_NO_FILE_OR_DIR 1
# define ERR_EMPTY_COMMAND 31
# define ERR_UNEXPECTED_TOKEN 2
# define ERR_MALLOC 3
# define SUCCESS 0

typedef struct s_token
{
	int				type;
	char			*value;
	struct s_token	*next;
	struct s_token	*prev;
}	t_token;

typedef struct s_garbage
{
	void				*ptr;
	struct s_garbage	*next;
}	t_garbage;

typedef struct s_cmdenv
{
	char	*cmd;
	char	**env;
}	t_cmdenv;

typedef struct s_lexer_meta
{
	int	*dollars;
}	t_lexer_meta;

typedef struct s_lexer
{
	t_lexer_meta	*meta;
	char			*str;
	int				i;
	int				quote;
}	t_lexer;

typedef struct s_exec
{
	char	*cmd_path;
	char	**cmd_args;
	int		out_type;
	char	*out_file;
	char	*in_file;
	int		in_type;
	int		in_fd;
	int		out_fd;
	int		type;
}	t_exec;

typedef struct s_state
{
	char			*cmd;
	char			*cwd;
	char			**env;
	char			status;
	int			cmd_amount;
	int				fd;
	int				*pipes;
	int				*forks;
	int				*exit_status;
	t_token			**tokens;
	t_garbage		*garbage;
	t_garbage		*last_garbage;
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
char	**ft_quote_split(char *s, char *c, t_state *state);
int		ft_strncmp(const char *s1, const char *s2, size_t n);
char	*ft_trim_quotes(char const *str, t_state *s, int n);
void	ft_free_tab(char **tab);
int		ft_is_empty(char *s);
char	**ft_arr_dup(char **arr);
int		ft_arr_len(char **arr);
void	ft_arr_push(char ***arr, char *str);
int		ft_arr_include(char **arr, char *str, \
	int (*cmp)(const char *, const char *));
int		ft_atoi(const char *str);
int		ft_isdigit(int c);
int		ft_isfullof(char *str, char c);
char	*ft_itoa(int n, t_state *s);
void	ft_write_error(char *cmd, char *error);
void	ft_error_hub(t_state *s, int err);
int		ft_is_starts_with_digit(t_exec *exec);

/* SIGNALS */
void	ft_sigint(int sig);
void	ft_sigquit(int sig);
void	ft_signals(void);

/* ENV CHECK */
void	ft_env_check(t_token *tmp, t_state *s);

/* GARBAGE */
void	ft_add_garbage(t_state *s, void *ptr);
void	ft_free_garbage(t_state *s);
void	ft_addarr_garbage(t_state *s, void **ptr);

/* NEW LEXER HELPERS */
void	ft_remove_char_by_index(char **str, int index, t_state *s);
char	*ft_joinstr_index(char *s1, char *s2, int start_index, t_state *s);

/* LEXER */
int		ft_lexer(t_state *s);
int		ft_count_pipes(char *cmd);
void	ft_remove_char_by_index(char **str, int index, t_state *s);

/* SHELL */
void	ft_start(t_state *s);

/* EXEC */
int		ft_execuator(t_state *s);
int		ft_amount_cmd(t_token **tokens);
int		ft_find_arg_amount(t_token *tokens);
int		ft_get_cmd_path(t_token *start_token, t_state *s, char **cmd_path);
void	close_pipes_all(int *pipes, int cmd_amount, int i);
void	mother_close_pipes_all(int *pipes, int cmd_amount);
int		ft_init_execs(t_state *s, t_exec **exec);
void	ft_init_pipes(t_state *s);
int		ft_dup_redictions(t_exec *exec, t_state *s);
int		ft_is_redirection(t_token *token);
int		close_redir_fd(t_exec *exec, int fd);
int		close_redir_pipe_fd(t_exec *exec, int *pipes, int cmd_amount, int i);
void	ft_run_redirects(t_state *s, t_exec **exec, int cmd_amount, int i);
void	ft_run_commands(t_state *s, t_exec **exec, int cmd_amount, int i);
void	exec_one_command(t_state *s, t_exec **exec);
void	ft_init_dupes(t_exec *exec, int *pipes, int cmd_amount, int i);
int		ft_open_check_files(t_exec *exec);

/* BUILTIN */
int		ft_is_builtin(char *value);
int		ft_execute_builtin(t_state *s, t_exec *exec);
int		ft_echo(t_exec *exec);
int		ft_export(t_exec *exec, t_state *s);
int		ft_pwd(t_state *s);
int		ft_cd(t_exec *exec, t_state *s);
int		ft_exit(t_exec *exec, t_state *s);
int		ft_unset(t_exec *exec, t_state *s);
int		ft_env(t_state *s);

/* HELPERS */
char	*ft_get_env(char **env, char *key);
void	ft_extend_str_by_index(char **str, int index, char c, t_state *s);
int		ft_env_key_cmp(const char *s1, const char *s2);

/* TOKENS */
void	ft_add_token(t_state *s, char *token, int type, int index);
t_token *ft_create_token(char *value, int type);
t_token	*ft_get_last_token(t_token *token);
int		ft_remove_tokens(t_token ***token, int (*f)(void *));
void	ft_free_tokens(t_token **token);
void	ft_init_prev_tokens(t_token **tokens);

/* ERROR */
void	ft_error(int err, char *str, int throw_exit);

/* DEBUG */
void	ft_print_tokens(t_token **token);
void	ft_print_tab(char **tab);
void	ft_signals(void);
void	ft_print_execs(t_exec **exec);
void	ft_print_exec(t_exec *exec);

#endif