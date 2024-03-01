/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_merge.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: egumus <egumus@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/01 14:49:53 by egumus            #+#    #+#             */
/*   Updated: 2024/03/01 14:54:00 by egumus           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_toggle_quote(t_lexer *l, char c)
{
	if (c == '\'' || c == '\"')
	{
		if (l->quote == QUOTE_NONE)
		{
			l->quote = c;
			return (1);
		}
		else if (l->quote == c)
		{
			l->take_it = 1;
			l->quote = QUOTE_NONE;
			return (2);
		}
	}
	return (0);
}

int	ft_merge_args_init(t_token *start_token, int *ta, t_token **wc)
{
	t_token	*tmp;

	tmp = start_token;
	while (tmp)
	{
		if (tmp->type == T_ARG)
		{
			if (!(*wc))
				*wc = tmp;
			(*ta)++;
		}
		tmp = tmp->next;
	}
	if (*ta == 0 || *ta == 1)
		return (1);
	return (0);
}

int	ft_merge_args_iterate(t_token **tmp, t_state *s, char **str)
{
	if ((*tmp)->type != T_ARG)
	{
		(*tmp) = (*tmp)->next;
		return (1);
	}
	if ((*tmp)->next && (*tmp)->next->type == T_ARG)
	{
		*str = ft_strjoin((*tmp)->value, ft_strdup((*tmp)->next->value, s), s);
		(*tmp)->value = *str;
		(*tmp)->next = (*tmp)->next->next;
	}
	else
		(*tmp) = (*tmp)->next;
	return (0);
}

void	ft_merge_args(t_token *start_token, t_state *s)
{
	int		total_args;
	t_token	*tmp;
	t_token	*will_change;
	char	*str;

	tmp = start_token;
	will_change = NULL;
	total_args = 0;
	str = NULL;
	if (ft_merge_args_init(start_token, &total_args, &will_change))
		return ;
	if (will_change == NULL)
		return ;
	while (tmp)
	{
		if (ft_merge_args_iterate(&tmp, s, &str))
			continue ;
	}
	will_change->value = str;
}
