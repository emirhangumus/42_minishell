/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: egumus <egumus@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/20 19:32:25 by egumus            #+#    #+#             */
/*   Updated: 2024/03/01 14:49:42 by egumus           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_lexer_init(t_lexer *l, t_state *s)
{
	l->i = 0;
	l->is_happend = 0;
	l->is_pipe_added = 0;
	l->take_it = 0;
	l->quote = QUOTE_NONE;
	l->str = NULL;
	l->sp = ft_quote_split(s->cmd, ' ', s);
	if (!l->sp)
		return (1);
	l->str = l->sp[l->i];
	return (0);
}

int	ft_lexer_iterate(t_state *s, t_lexer *l)
{
	while (l->str)
	{
		if (l->i == 0 || l->is_pipe_added)
		{
			ft_create_token(&s->tokens, ft_trim_quotes(l->str, s), T_CMD);
			if (l->is_pipe_added && l->i != 0)
				l->is_pipe_added = 0;
			l->str = l->sp[++(l->i)];
			continue ;
		}
		l->is_happend = 0;
		l->take_it = 0;
		if (ft_lexer_bychar(s, l))
		{
			printf("Error: quote not closed\n");
			return (1);
		}
		l->str = l->sp[++(l->i)];
	}
	return (ft_remove_tokens(&s->tokens, (int (*)(void *))ft_is_empty), 0);
}

int	ft_lexer(t_state *s)
{
	t_lexer	*l;

	l = (t_lexer *)malloc(sizeof(t_lexer));
	if (!l)
		return (1);
	if (ft_lexer_init(l, s))
		return (1);
	if (ft_lexer_iterate(s, l))
		return (1);
	return (0);
}
