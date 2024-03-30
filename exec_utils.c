/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: burkaya <burkaya@student.42istanbul.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/01 12:09:01 by burkaya           #+#    #+#             */
/*   Updated: 2024/03/30 14:21:24 by burkaya          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_amount_cmd(t_token **tokens)
{
	int		amount;
	int		i;
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

void	mother_close_pipes_all(t_state *s)
{
	int	j;

	j = 0;
	while (j < s->cmd_amount * 2)
	{
		close(s->pipes[j]);
		j++;
	}
}
