/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_merge.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: egumus <egumus@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/01 14:49:53 by egumus            #+#    #+#             */
/*   Updated: 2024/03/08 02:08:23 by egumus           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// int	ft_toggle_quote_merge(t_lexer *l, char c)
// {
// 	if (c == '\'' || c == '\"')
// 	{
// 		if (l->seen_quote_type == QUOTE_NONE)
// 		{
// 			l->seen_quote_type = c;
// 			return (1);
// 		}
// 		else if (l->seen_quote_type == c)
// 		{
// 			l->take_it = 1;
// 			l->seen_quote_type = QUOTE_NONE;
// 			return (2);
// 		}
// 	}
// 	return (0);
// }

int	ft_l_merge_tokens_init(t_token *start_token, t_token **wc, int merge_type)
{
	t_token *tmp;
	int     total_args;

	total_args = 0;
	tmp = start_token;
	while (tmp)
	{
		if (tmp->type == merge_type)
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

int	ft_merge_args_iterate(t_token **tmp, t_state *s, char **str, int merge_type)
{
	if ((*tmp)->next && (*tmp)->next->type == merge_type)
	{
		*str = ft_strjoin(ft_strdup((*tmp)->value, s), ft_strdup((*tmp)->next->value, s), s);
		(*tmp)->value = *str;
		(*tmp)->next = (*tmp)->next->next;
	}
	else
		(*tmp) = (*tmp)->next;
	return (0);
}

void	ft_l_merge_tokens(t_token *start_token, t_state *s, int merge_type)
{
	int		total_args;
	t_token	*tmp;
	t_token	*will_change;
	char	*str;

	tmp = start_token;
	will_change = NULL;
	total_args = 0;
	str = NULL;
	total_args = ft_l_merge_tokens_init(start_token, &will_change, merge_type);
	if (will_change == NULL)
		return ;
	while (tmp)
	{
		if (ft_merge_args_iterate(&tmp, s, &str, merge_type))
			continue ;
	}
	if (str != NULL)
		will_change->value = str;
}
