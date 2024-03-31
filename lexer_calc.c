/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_calc.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: egumus <egumus@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/31 22:18:24 by egumus            #+#    #+#             */
/*   Updated: 2024/03/31 22:23:23 by egumus           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	ft_calc_dollars_helper(t_lexer *l, char *str)
{
	int	i;
	int	quote;
	int	count;

	i = -1;
	count = 0;
	quote = QUOTE_NONE;
	while (str[++i])
	{
		if (quote == QUOTE_NONE && (str[i] == '\'' || str[i] == '\"'))
			quote = str[i];
		else if (quote == str[i])
			quote = QUOTE_NONE;
		if (str[i] == '$')
		{
			if ((quote == QUOTE_NONE || quote == QUOTE_TWO)
				&& (ft_is_valid_env_key_char(str[i + 1]) || str[i + 1] == '?'))
				l->meta->dollars[count] = 1;
			else
				l->meta->dollars[count] = 0;
			count++;
		}
	}
}

void	ft_calc_dollars(char *str, t_lexer *l, t_state *s)
{
	int	i;
	int	count;

	i = 0;
	count = 0;
	while (str[i])
	{
		if (str[i] == '$')
			count++;
		i++;
	}
	l->meta = malloc(sizeof(t_lexer_meta));
	if (!l->meta)
		return ;
	ft_add_garbage(s, l->meta);
	l->meta->dollars = malloc(sizeof(int) * count);
	if (!l->meta->dollars)
		return ;
	ft_add_garbage(s, l->meta->dollars);
	ft_calc_dollars_helper(l, str);
}
