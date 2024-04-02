/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: egumus <egumus@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/14 22:59:11 by egumus            #+#    #+#             */
/*   Updated: 2024/04/03 01:05:25 by egumus           ###   ########.fr       */
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
# include <sys/stat.h>
# include <errno.h>
# include <termios.h>

# define T_CMD 1
# define T_ARG 2
# define T_LREDIR 3
# define T_RREDIR 4
# define T_RAPPEND 5
# define T_LAPPEND 6

# define QUOTE_NONE 0
# define QUOTE_ONE 39
# define QUOTE_TWO 34

# define CMD_PATH 19
# define CMD_BUILTIN 20
# define CMD_WITHOUT_CMD 21

# define COLOR_RED "\x1b[31m"
# define COLOR_GREEN "\x1b[32m"
# define COLOR_BLUE "\x1b[34m"
# define COLOR_RESET "\x1b[0m"

# define ERR_CMD_NOT_FOUND 127
# define ERR_IS_A_DIRECTORY 126
# define ERR_PERMISSION_DENIED 1261
# define ERR_NUMERIC_ARG 255
# define ERR_PIPE_INIT 124
# define ERR_NOT_VALID_IDFR 11
# define ERR_NOT_A_DIRECTORY 122
# define ERR_NO_FILE_OR_DIR 1
# define ERR_EMPTY_COMMAND 1271
# define ERR_UNEXPECTED_TOKEN 258
# define ERR_MALLOC 3
# define ERR_UNCOMPLETED_REDIRECT 4
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
	int				**forced_arg_range;
	char			*original;
	int				k;
	int				m;
	int				current_pipe_index;
	int				rm1;
	int				rm2;
	char			*key;
	char			*value;
	int				dollar_counter;
}	t_lexer;

typedef struct s_exec
{
	t_token	**t_ptr1;
	t_token	*t_ptr2;
	char	*cmd_path;
	char	**cmd_args;
	char	**heredocs;
	int		err_no;
	char	*err_value;
	int		is_without_cmd;
	int		count_heredocs;
	int		here_doc_idx;
	int		out_type;
	char	*out_file;
	char	*in_file;
	int		in_type;
	int		should_run;
	char	*is_here_doc;
	int		in_fd;
	int		out_fd;
	int		type;
}	t_exec;

typedef struct s_state
{
	char			*exec_name;
	char			*cmd;
	char			*cwd;
	char			**env;
	int				status;
	int				cmd_amount;
	int				fd;
	int				*pipes;
	int				*forks;
	int				*exit_status;
	t_token			**tokens;
	t_garbage		*garbage;
	t_garbage		*last_garbage;
}	t_state;

typedef struct s_quote_split
{
	int		seen_quote_type;
	int		i;
	int		j;
	int		k;
	char	*s;
	char	*c;
}	t_quote_split;

typedef struct s_arr_add_by_index
{
	int		len;
	char	**new_arr;
	int		i;
	int		j;
}	t_arr_add_by_index;

extern int	g_qsignal;

size_t	ft_strlen(const char *s);
char	*ft_strdup(char *str, t_state *s);
char	**ft_split(char *s, char c, t_state *state);
char	*ft_strjoin(char const *s1, char const *s2, t_state *s);
int		ft_strcmp(const char *s1, const char *s2);
char	*ft_substr(const char *s, unsigned int start, size_t len, t_state *st);
char	*ft_strchr(const char *s, int c);
char	**ft_quote_split(char *s, char *c, t_state *state);
int		ft_strncmp(const char *s1, const char *s2, size_t n);
void	ft_free_tab(char **tab);
int		ft_is_empty(char *s);
char	**ft_arr_dup(char **arr, t_state *s);
int		ft_arr_len(char **arr);
int		ft_arr_include(char **arr, char *str, \
	int (*cmp)(const char *, const char *));
int		ft_atoi(const char *str);
int		ft_isdigit(int c);
int		ft_isfullof(char *str, char c);
char	*ft_itoa(int n, t_state *s);
int		ft_is_starts_with_digit(char *value);
int		ft_isallnum(char *str);
char	**ft_add_env(char **env, char *key, char *value, t_state *s);
void	ft_arr_add_by_index(char ***arr, char *str, int index, t_state *s);
void	ft_arr_remove_by_index(char ***arr, int index, t_state *s);
int		ft_is_valid_env_key_char(char c);
char	*ft_strrchr(const char *s, int c);
int		ft_isalnum(int c);
char	*ft_joinstr_index(char *s1, char *s2, int start_index, t_state *s);
void	ft_remove_char_by_index(char **str, int index, t_state *s);
void	ft_remove_key(char **str, int start_index, int end_index, t_state *s);
char	**ft_split_merge(char **split, t_state *s);
char	**ft_free_prevs(char **arr, int i);

void	ft_signals(void);

void	ft_add_garbage(t_state *s, void *ptr);
void	ft_free_garbage(t_state *s);
void	ft_addarr_garbage(t_state *s, void **ptr);

int		ft_lexer(t_state *s);
int		ft_merge_args(int index, t_state *s, t_lexer *l, char ***split);
int		ft_count_pipes(char *cmd);
void	ft_remove_char_by_index(char **str, int index, t_state *s);
void	ft_calc_dollars(char *str, t_lexer *l, t_state *s);
int		ft_add_envs_as_arg_or_cmd(int index, t_state *s, char ***split);
int		ft_is_redirect(char *str, char *original);
int		ft_remove_tokens(t_token ***token, int (*f)(void *));
void	ft_redirect_arrange(t_token **tokens);
int		ft_lexer_validate(t_state *s);
int		ft_check_invalid_pipes(char *cmd, int si);
char	**ft_split_specials(char *str, t_state *s);

void	ft_start(t_state *s);

int		ft_execuator(t_state *s);
int		ft_amount_cmd(t_token **tokens);
int		ft_find_arg_amount(t_token *tokens);
void	close_pipes_all(int *pipes, int cmd_amount, int i);
void	mother_close_pipes_all(t_state *s);
int		ft_init_execs(t_state *s, t_exec **exec);
void	ft_init_pipes(t_state *s);
int		ft_dup_redictions(t_exec *exec, t_state *s);
int		ft_is_redirection(t_token *token);
int		close_redir_fd(t_exec *exec, int fd1, int fd2);
int		close_redir_pipe_fd(t_state *s, t_exec *exec, int i);
void	ft_run_redirects(t_state *s, t_exec **exec, int i);
void	ft_run_commands(t_state *s, t_exec **exec, int i);
int		exec_one_command(t_state *s, t_exec **exec);
void	ft_init_dupes(t_state *s, t_exec *exec, int i);
int		ft_open_check_files(t_exec *exec, int status, t_state *s);
int		get_all_cmd(t_exec *exec, t_state *s, t_token *tmp, t_token *tmp1);
int		ft_init_redirections(t_token *tokens, t_exec *exec, t_state *s);
void	ft_heredoc(t_exec *exec);
void	ft_exec_hub(t_state *s, t_exec **exec);
int		isredwocmd(t_token *tokens, int cmd_amount, int j);
char	*ft_is_here_doc(t_token *token);
int		ft_count_heredocs(t_token *token);
int		isredwocmd(t_token *tokens, int cmd_amount, int j);
void	ft_set_exec_err(t_exec *exec, int err, char *value);
void	ft_print_exec_errors(t_state *s, t_exec **exec);
void	ft_export_add_key_value(char *str, t_state *s, int j);

int		ft_is_builtin(char *value);
int		ft_execute_builtin(t_state *s, t_exec *exec);
int		ft_echo(t_exec *exec);
int		ft_export(t_exec *exec, t_state *s);
int		ft_pwd(t_state *s);
int		ft_cd(t_exec *exec, t_state *s);
int		ft_exit(t_exec *exec, t_state *s);
int		ft_unset(t_exec *exec, t_state *s);
int		ft_env(t_state *s);

char	*ft_get_env(char **env, char *key);
void	ft_extend_str_by_index(char **str, int index, char c, t_state *s);
int		ft_env_key_cmp(const char *s1, const char *s2);

void	ft_add_token(t_state *s, char *token, int type, int index);
t_token	*ft_create_token(char *value, int type);
t_token	*ft_get_last_token(t_token *token);
int		ft_remove_tokens(t_token ***token, int (*f)(void *));
void	ft_free_tokens(t_token **token);
void	ft_init_prev_tokens(t_token **tokens);

void	ft_error(int err, char *str, int throw_exit);

void	rl_replace_line(const char *text, int clear_undo);

#endif