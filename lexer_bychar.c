/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_bychar.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: egumus <egumus@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/01 14:47:25 by egumus            #+#    #+#             */
/*   Updated: 2024/03/03 21:35:26 by egumus           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_take_it(t_lexer *l, t_state *s, int *i, int *j)
{
	ft_create_token(&s->tokens, \
		ft_substr(l->str, *j, *i - *j + 1, s), T_ARG);
	*j = *i + 1;
	l->take_it = 0;
	l->is_happend = 0;
	l->is_pipe_added = 0;
	return (0);
}

void	ft_lexer_bychar_pipe(t_state *s, t_lexer *l, int *i, int *j)
{
	if (l->quote == QUOTE_NONE && l->str[*i] == '|')
	{
		if (*i != 0 && !l->is_pipe_added)
			ft_create_token(&s->tokens, \
				ft_trim_quotes(ft_substr(l->str, *j, *i - *j, s), s, 1), T_ARG);
		ft_create_token(&s->tokens, ft_strdup("|", s), T_PIPE);
		l->is_pipe_added = 1;
		*j = *i + 1;
	}
}

int	ft_lexer_bychar_iterate(t_state *s, t_lexer *l, int	*i, int	*j)
{
	if (l->quote == QUOTE_NONE && \
		(l->str[*i + 1] == '\'' || l->str[*i + 1] == '\"'))
		l->take_it = 1;
	if (ft_toggle_quote(l, l->str[*i]) == 1)
	{
		*j = *i;
		return (1);
	}
	if (l->quote == QUOTE_NONE && l->is_pipe_added)
	{
		*j = *i;
		while (l->str[*i])
			(*i)++;
		ft_create_token(&s->tokens, \
			ft_trim_quotes(ft_substr(l->str, *j, *i - *j + 1, s), s, 1), T_CMD);
		l->is_pipe_added = 0;
		return (2);
	}
	ft_lexer_bychar_pipe(s, l, i, j);
	if (!l->str[*i + 1] && l->quote == QUOTE_NONE && !l->is_pipe_added)
		l->take_it = 1;
	if (l->take_it)
		ft_take_it(l, s, i, j);
	return (0);
}

int	ft_lexer_bychar(t_state *s, t_lexer *l)
{
	int		i;
	int		j;
	int		ret;
	t_token	*tmp;

	i = -1;
	j = 0;
	ret = 0;
	tmp = ft_get_last_token(s->tokens);
	while (l->str[++i])
	{
		ret = ft_lexer_bychar_iterate(s, l, &i, &j);
		if (ret == 1)
			continue ;
		if (ret == 2)
			break ;
	}
	if (l->quote != QUOTE_NONE)
		return (1);
	return (ft_merge_args(tmp->next, s), 0);
}
