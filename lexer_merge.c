/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_merge.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: egumus <egumus@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/01 14:49:53 by egumus            #+#    #+#             */
/*   Updated: 2024/03/05 06:15:56 by egumus           ###   ########.fr       */
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

int	ft_merge_args_init(t_token *start_token, t_token **wc)
{
	t_token *tmp;
	int     total_args;

	total_args = 0;
	tmp = start_token;
	while (tmp)
	{
		if (tmp->type == T_ARG)
		{
			if (!(*wc))
				*wc = tmp;
			total_args++;
		}
		tmp = tmp->next;
	}
	if (total_args == 0 || total_args == 1)
	{
		if (total_args == 1)
			return (-2);
		return (-1);
	}
	return (total_args);
}

void	ft_set_envs(t_token *t, t_state *s)
{
	t_token *tmp;

	tmp = t;
	while (tmp)
	{
		if (tmp->type == T_ARG)
		{
			ft_env_check(tmp, s);
		}
		tmp = tmp->next;
	}
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
		*str = ft_strjoin(ft_trim_quotes(ft_strdup((*tmp)->value, s), s, 1), ft_trim_quotes(ft_strdup((*tmp)->next->value, s), s, 1), s);
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
	total_args = ft_merge_args_init(start_token, &will_change);
	if (will_change == NULL)
		return ;
	if (total_args < 0)
	{
		if (total_args == -2)
		{
			ft_env_check(will_change, s);
			will_change->value = ft_trim_quotes(ft_strdup(will_change->value, s), s, 1);
		}
		if (total_args == -1)
			will_change->value = ft_strdup("", s);
		return ;
	}
	ft_set_envs(tmp, s);
	while (tmp)
	{
		if (ft_merge_args_iterate(&tmp, s, &str))
			continue ;
	}
	will_change->value = str;
}
