/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: egumus <egumus@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/20 19:32:25 by egumus            #+#    #+#             */
/*   Updated: 2024/03/06 17:48:45 by egumus           ###   ########.fr       */
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

int	ft_quote_remover_cmd(t_state *s, t_lexer *l)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (l->str[i])
	{
		if (ft_lexer_bychar_iterate(s, l, &i, &j) == 2)
			return (0);
		if (l->take_it)
			ft_take_it(l, s, &i, &j);
		if (l->is_happend)
			continue ;
		if (l->quote == QUOTE_NONE && l->str[i] == ' ')
		{
			ft_create_token(&s->tokens, \
				ft_trim_quotes(ft_substr(l->str, j, i - j, s), s, 1), T_ARG);
			j = i + 1;
		}
		i++;
	}
	if (l->quote != QUOTE_NONE)
	{
		printf("Error: quote not closed\n");
		return (1);
	}
	if (l->str[i - 1] != ' ')
		ft_create_token(&s->tokens, \
			ft_trim_quotes(ft_substr(l->str, j, i - j, s), s, 1), T_ARG);
	return (0);
}

int	ft_lexer_iterate(t_state *s, t_lexer *l)
{
	while (l->str)
	{
		if (l->i == 0 || l->is_pipe_added)
		{
			ft_create_token(&s->tokens, ft_trim_quotes(l->str, s, 1), T_CMD);
			// ft_env_check(s->tokens, s);
			// ft_quote_remover_cmd(s, l);
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
	free(l);
	return (0);
}
