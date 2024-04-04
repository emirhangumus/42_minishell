/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_path.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: burkaya <burkaya@student.42istanbul.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/04 23:36:33 by burkaya           #+#    #+#             */
/*   Updated: 2024/04/04 23:38:04 by burkaya          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_find_absoulute_path(t_token *start_token, t_exec *exec)
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
		return (ft_set_exec_err(exec, ERR_NO_FILE_OR_DIR, \
			start_token->value), 127);
	if (access(start_token->value, X_OK))
		return (ft_set_exec_err(exec, ERR_PERMISSION_DENIED, \
			start_token->value), 126);
	exec->cmd_path = start_token->value;
	return (0);
}

int	ft_find_rela_path(char **paths, t_token *token, \
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

int	ft_is_in_cwd(t_state *s, t_exec *exec, char *cmd_name)
{
	char		*path;
	struct stat	buf;

	path = ft_strjoin(s->cwd, "/", s);
	path = ft_strjoin(path, cmd_name, s);
	stat(path, &buf);
	if (S_ISDIR(buf.st_mode))
		return (ft_set_exec_err(exec, ERR_IS_A_DIRECTORY, path), 1);
	if (!access(path, F_OK) && !access(path, X_OK))
	{
		exec->cmd_path = path;
		return (0);
	}
	return (1);
}
