/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: burkaya <burkaya@student.42istanbul.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/01 12:09:01 by burkaya           #+#    #+#             */
/*   Updated: 2024/03/03 15:54:14 by burkaya          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_amount_cmd(t_token *tokens)
{
	int	amount;

	amount = 0;
	while (tokens)
	{
		if (tokens->type == T_CMD)
			amount++;
		tokens = tokens->next;
	}
	return (amount);
}

int	ft_find_arg_amount(t_token *tokens)
{
	int	amount;

	amount = 0;
	tokens = tokens->next;
	while (tokens)
	{
		if (tokens->type == T_ARG)
			amount++;
		else
			break ;
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
