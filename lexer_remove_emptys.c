/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_remove_emptys.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: burkaya <burkaya@student.42istanbul.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/31 22:38:46 by egumus            #+#    #+#             */
/*   Updated: 2024/04/05 00:27:27 by burkaya          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	ft_remove_tokens_command_helper(t_token ***token, t_token **tmp, \
	t_token **prev, int (*f)(void *))
{
	if (f((*tmp)->value))
	{
		if ((*tmp)->type == T_CMD && (*tmp)->remove == 1)
		{
			free((*tmp)->value);
			(*tmp)->value = ft_strdup("a", NULL);
			(*tmp)->value[0] = 1;
		}
		else if ((*tmp)->type == T_ARG && (*tmp)->remove == 1)
			(*tmp)->value[0] = 0;
		else
		{
			if ((*prev))
				(*prev)->next = (*tmp)->next;
			else
				**token = (*tmp)->next;
			free((*tmp)->value);
			free((*tmp));
			*tmp = *prev;
		}
	}
}

static int	ft_remove_tokens_command(t_token **token, int (*f)(void *), int i)
{
	t_token	*tmp;
	t_token	*prev;

	tmp = *token;
	prev = NULL;
	while (tmp)
	{
		ft_remove_tokens_command_helper(&token, &tmp, &prev, f);
		if (!tmp)
		{
			if (i == 0 && *token)
				(*token)->type = T_CMD;
			tmp = *token;
			continue ;
		}
		prev = tmp;
		tmp = tmp->next;
		i++;
	}
	return (SUCCESS);
}

int	ft_remove_tokens(t_token ***token, int (*f)(void *))
{
	int		i;
	t_token	**tmp;
	int		err;

	i = 0;
	err = 0;
	tmp = *token;
	while (tmp[i])
	{
		err = ft_remove_tokens_command(&tmp[i], f, i);
		if (err)
			return (err);
		i++;
	}
	return (SUCCESS);
}
