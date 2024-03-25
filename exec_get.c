/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_get.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: burkaya <burkaya@student.42istanbul.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/25 17:37:15 by burkaya           #+#    #+#             */
/*   Updated: 2024/03/25 18:08:16 by burkaya          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_find_absoulute_path(t_token *start_token, char **cmd_path)
{
	struct stat	buf;

	stat(start_token->value, &buf);
	if (errno == EACCES)
		return (ft_error(ERR_PERMISSION_DENIED, start_token->value, 0), 1);
	if (S_ISDIR(buf.st_mode))
		return (ft_error(ERR_IS_A_DIRECTORY, start_token->value, 0), \
			ERR_IS_A_DIRECTORY);
	if (access(start_token->value, F_OK))
		return (ft_error(ERR_PERMISSION_DENIED, start_token->value, 0), \
			ERR_CMD_NOT_FOUND);
	if (access(start_token->value, X_OK))
		return (ft_error(ERR_PERMISSION_DENIED, start_token->value, 0), \
			ERR_PERMISSION_DENIED);
	*cmd_path = start_token->value;
	return (0);
}

int	ft_find_rela_path(char **paths, t_token *token, t_state *s, char **cmd_path)
{
	char		*path;
	int			i;
	char		*env;
	struct stat	buf;

	env = ft_get_env(s->env, "PATH");
	i = -1;
	while (paths[++i])
	{
		path = ft_strjoin(paths[i], "/", s);
		path = ft_strjoin(path, token->value, s);
		stat(path, &buf);
		if (S_ISDIR(buf.st_mode))
			return (ft_error(ERR_IS_A_DIRECTORY, path, 0), 1);
		if (!access(path, F_OK) && !access(path, X_OK))
		{
			*cmd_path = path;
			return (0);
		}
	}
	return (ft_error(ERR_CMD_NOT_FOUND, token->value, 0), ERR_CMD_NOT_FOUND);
}

int	ft_get_cmd_path(t_token *start_token, t_state *s, char **cmd_path)
{
	char	*env;
	char	**paths;
	int		i;

	env = ft_get_env(s->env, "PATH");
	paths = ft_split(env, ':', s);
	i = -1;
	if (ft_strchr(start_token->value, '/') != NULL)
		return (ft_find_absoulute_path(start_token, cmd_path));
	return (ft_find_rela_path(paths, start_token, s, cmd_path));
}

char	**ft_get_args(t_state *s, t_token *tokens, char *cmd_name)
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
	ft_add_garbage(s, exec);
	if (!exec)
		return (1);
	if (ft_is_builtin(tmp->value))
		exec->type = CMD_BUILTIN;
	else
		exec->type = CMD_PATH;
	if (exec->type == CMD_PATH)
		err = ft_get_cmd_path(tmp, s, &exec->cmd_path);
	if (err && !(exec->type == CMD_BUILTIN))
		return (err);
	ft_init_redirections(tmp1, exec, s);
	exec->cmd_args = ft_get_args(s, tmp1, tmp->value);
	return (0);
}