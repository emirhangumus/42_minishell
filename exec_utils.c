/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: burkaya <burkaya@student.42istanbul.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/01 12:09:01 by burkaya           #+#    #+#             */
/*   Updated: 2024/03/25 13:24:42 by burkaya          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_amount_cmd(t_token **tokens)
{
	int	amount;
	int	i;
	t_token	*tmp;

	i = 0;
	amount = 0;
	while (tokens[i])
	{
		tmp = tokens[i];
		while (tmp)
		{
			if (tmp->type == T_CMD)
				amount++;
			tmp = tmp->next;
		}
		i++;
	}
	return (amount);
}

int	ft_find_arg_amount(t_token *tokens)
{
	int	amount;
	int	i;

	i = 0;
	amount = 0;
	while (tokens)
	{
		if (i == 0 && tokens->type == T_ARG)
			amount++;
		if (i != 0 && tokens->type == T_ARG && !ft_is_redirection(tokens->prev))
			amount++;
		tokens = tokens->next;
		i++;
	}
	return (amount);
}

int	ft_get_cmd_path(t_token *start_token, t_state *s, char **cmd_path)
{
	char	*env;
	char	**paths;
	char	*path;
	int		i;

	i = -1;
	if (ft_strchr(start_token->value, '/') != NULL)
	{
		struct stat buf;
		stat(start_token->value, &buf);
		if (errno == EACCES)
			return (ft_error(ERR_PERMISSION_DENIED, start_token->value, 0), 1);
		if (S_ISDIR(buf.st_mode))
			return (ft_error(ERR_IS_A_DIRECTORY, start_token->value, 0), 1);
		if (access(start_token->value, F_OK))
			return (ft_error(ERR_PERMISSION_DENIED, start_token->value, 0), ERR_CMD_NOT_FOUND);
		if (access(start_token->value, X_OK))
			return (ft_error(ERR_PERMISSION_DENIED, start_token->value, 0), ERR_PERMISSION_DENIED);
		*cmd_path = start_token->value;
		return (0);
	}
	env = ft_get_env(s->env, "PATH");
	paths = ft_split(env, ':', s);
	while (paths[++i])
	{
		path = ft_strjoin(paths[i], "/", s);
		path = ft_strjoin(path, start_token->value, s);
		struct stat buf;
		stat(path, &buf);
		if (S_ISDIR(buf.st_mode))
			return (ft_error(ERR_IS_A_DIRECTORY, path, 0), 1);
		if (!access(path, F_OK) && !access(path, X_OK))
		{
			*cmd_path = path;
			return (0);
		}
	}
	return (ft_error(ERR_CMD_NOT_FOUND, start_token->value, 0), ERR_CMD_NOT_FOUND);
	return (1);
}

void	close_pipes_all(int *pipes, int cmd_amount, int i)
{
	int	j;

	j = -1;
	while (++j < cmd_amount * 2)
	{
		if ((i == 0 && j != i * 2 + 1)
			|| (i == cmd_amount - 1 && j != (i - 1) * 2)
			|| (i != 0 && i != cmd_amount - 1 && j != (i - 1) * 2 \
				&& j != i * 2 + 1))
		{
			close(pipes[j]);
		}
	}
}

void	mother_close_pipes_all(int *pipes, int cmd_amount)
{
	int	j;

	j = 0;
	while (j < cmd_amount * 2)
	{
		close(pipes[j]);
		j++;
	}
}
