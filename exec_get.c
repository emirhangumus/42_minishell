/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_get.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: egumus <egumus@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/25 17:37:15 by burkaya           #+#    #+#             */
/*   Updated: 2024/04/03 01:00:42 by egumus           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	ft_find_absoulute_path(t_token *start_token, t_exec *exec)
{
	struct stat	buf;

	stat(start_token->value, &buf);
	exec->cmd_path = NULL;
	if (errno == EACCES)
		return (ft_set_exec_err(exec, ERR_PERMISSION_DENIED, \
			start_token->value), 1);
	if (S_ISDIR(buf.st_mode))
		return (ft_set_exec_err(exec, ERR_IS_A_DIRECTORY, start_token->value), \
			ERR_IS_A_DIRECTORY);
	if (access(start_token->value, F_OK))
		return (ft_set_exec_err(exec, ERR_PERMISSION_DENIED, \
			start_token->value), ERR_CMD_NOT_FOUND);
	if (access(start_token->value, X_OK))
		return (ft_set_exec_err(exec, ERR_PERMISSION_DENIED, \
			start_token->value), 126);
	exec->cmd_path = start_token->value;
	return (0);
}

static int	ft_find_rela_path(char **paths, t_token *token, \
	t_state *s, t_exec *exec)
{
	char		*path;
	int			i;
	struct stat	buf;

	i = -1;
	while (paths[++i])
	{
		path = ft_strjoin(paths[i], "/", s);
		path = ft_strjoin(path, token->value, s);
		stat(path, &buf);
		if (S_ISDIR(buf.st_mode))
			return (ft_set_exec_err(exec, ERR_IS_A_DIRECTORY, path), 1);
		if (!access(path, F_OK) && !access(path, X_OK))
		{
			exec->cmd_path = path;
			return (0);
		}
	}
	return (ft_set_exec_err(exec, ERR_CMD_NOT_FOUND, \
		token->value), ERR_CMD_NOT_FOUND);
}

static int	ft_get_cmd_path(t_token *start_token, t_state *s, t_exec *exec)
{
	char	*env;
	char	**paths;

	env = ft_get_env(s->env, "PATH");
	if (ft_strchr(start_token->value, '/') != NULL)
		return (ft_find_absoulute_path(start_token, exec));
	if (!env)
		return (ft_error(ERR_CMD_NOT_FOUND, start_token->value, 0), \
			ERR_CMD_NOT_FOUND);
	paths = ft_split(env, ':', s);
	return (ft_find_rela_path(paths, start_token, s, exec));
}

static char	**ft_get_args(t_state *s, t_token *tokens, char *cmd_name)
{
	char	**args;
	int		i;
	int		arg_amount;

	i = 0;
	arg_amount = ft_find_arg_amount(tokens);
	args = malloc(sizeof(char *) * (arg_amount + 2));
	ft_add_garbage(s, args);
	args[0] = ft_strdup(cmd_name, s);
	args[arg_amount + 1] = NULL;
	while (tokens)
	{
		if (tokens->type == T_ARG && !ft_is_redirection(tokens->prev))
			args[++i] = ft_strdup(tokens->value, s);
		tokens = tokens->next;
	}
	return (args);
}

int	get_all_cmd(t_exec *exec, t_state *s, t_token *tmp, t_token *tmp1)
{
	int	err;

	err = 0;
	if (!exec)
		return (1);
	ft_add_garbage(s, exec);
	if (exec->is_without_cmd)
	{
		exec->type = CMD_WITHOUT_CMD;
		exec->cmd_path = "without_cmd";
	}
	exec->is_here_doc = ft_is_here_doc(tmp1);
	exec->count_heredocs = ft_count_heredocs(tmp1);
	err = ft_init_redirections(tmp1, exec, s);
	if (ft_is_builtin(tmp->value))
		exec->type = CMD_BUILTIN;
	if (exec->type == CMD_PATH)
		err = ft_get_cmd_path(tmp, s, exec);
	if (err && !(exec->type == CMD_BUILTIN))
		return (err);
	exec->cmd_args = ft_get_args(s, tmp1, tmp->value);
	if (err)
		return (err);
	return (0);
}
