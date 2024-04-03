/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_heredoc.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: burkaya <burkaya@student.42istanbul.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/01 10:20:38 by egumus            #+#    #+#             */
/*   Updated: 2024/04/03 12:49:37 by burkaya          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*ft_is_here_doc(t_token *token)
{
	while (token)
	{
		if (token->type == T_LAPPEND)
			return (token->next->value);
		token = token->next;
	}
	return (NULL);
}

int	ft_count_heredocs(t_token *token)
{
	int	count;

	count = 0;
	while (token)
	{
		if (token->type == T_LAPPEND)
			count++;
		token = token->next;
	}
	return (count);
}

int	isredwocmd_helper(t_token *tokens, int i, t_token *tmp)
{
	if (i == 0)
	{
		tmp = tokens;
		while (tmp)
		{
			if (ft_is_redirection(tmp))
				return (1);
			tmp = tmp->next;
		}
	}
	return (0);
}

int	isredwocmd(t_token *tokens, int cmd_amount, int j)
{
	int		i;
	t_token	*tmp;

	i = 0;
	tmp = tokens;
	if (j >= cmd_amount)
		return (0);
	while (tmp)
	{
		if (tmp->type == T_CMD)
			i++;
		if (i != 0)
			return (0);
		tmp = tmp->next;
	}
	return (isredwocmd_helper(tokens, i, tmp));
}
