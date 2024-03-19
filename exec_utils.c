/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: burkaya <burkaya@student.42istanbul.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/01 12:09:01 by burkaya           #+#    #+#             */
/*   Updated: 2024/03/19 20:51:44 by burkaya          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_amount_cmd(t_token **tokens)
{
	int	i;

	i = 0;
	while (tokens[i])
		i++;
	return (i);
}

int	ft_find_arg_amount(t_token *tokens)
{
	// echo adasd ad as 
	int	amount;

	amount = 0;
	while (tokens)
	{
		if (tokens->type == T_ARG)
			amount++;
		tokens = tokens->next;
	}
	return (amount);
}

char	*ft_get_cmd_path(t_token *start_token, t_state *s)
{
	char	*cmd_path;
	char	**paths;
	char	*path;
	int		i;

	i = 0;
	if (ft_strchr(start_token->value, '/') != NULL)
	{
		if (access(start_token->value, F_OK) == 0)
			return (start_token->value);
		else
			return (NULL);
	}
	path = ft_get_env(s->env, "PATH");
	paths = ft_split(path, ':', s);
	while (paths[i])
	{
		cmd_path = ft_strjoin(paths[i], "/", s);
		cmd_path = ft_strjoin(cmd_path, start_token->value, s);
		if (access(cmd_path, F_OK) == 0)
			return (cmd_path);
		i++;
	}
	return (NULL);
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
