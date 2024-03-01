/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: egumus <egumus@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/20 19:32:25 by egumus            #+#    #+#             */
/*   Updated: 2024/03/01 14:38:48 by egumus           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// void	ft_rm_quote_len(int *quote_type, char *s, int *i, int *j)
// {
// 	while (s[*i] != '\0')
// 	{
// 		if (s[*i] == '\'' || s[*i] == '\"')
// 		{
// 			if (*quote_type == QUOTE_NONE)
// 			{
// 				*quote_type = s[*i];
// 				(*i)++;
// 				continue ;
// 			}
// 			else if (*quote_type == s[*i])
// 			{
// 				*quote_type = QUOTE_NONE;
// 				(*i)++;
// 				continue ;
// 			}
// 		}
// 		(*i)++;
// 		(*j)++;
// 	}
// }

// void	ft_rm_quote_iterate(char *s, char **new, int *i, int *j)
// {
// 	int	quote_type;

// 	quote_type = QUOTE_NONE;
// 	while (s[*i])
// 	{
// 		if (s[*i] == '\'' || s[*i] == '\"')
// 		{
// 			if (quote_type == QUOTE_NONE)
// 			{
// 				quote_type = s[*i];
// 				(*i)++;
// 				continue ;
// 			}
// 			else if (quote_type == s[*i])
// 			{
// 				quote_type = QUOTE_NONE;
// 				(*i)++;
// 				continue ;
// 			}
// 		}
// 		*new[*j] = s[*i];
// 		(*i)++;
// 		(*j)++;
// 	}
// 	*new[*j] = '\0';
// }

// char	*ft_remove_quotes(char *s, t_state *state)
// {
// 	int		i;
// 	int		j;
// 	int		quote_type;
// 	char	*new;

// 	i = 0;
// 	j = 0;
// 	quote_type = QUOTE_NONE;
// 	ft_rm_quote_len(&quote_type, s, &i, &j);
// 	new = (char *)malloc(sizeof(char) * (j + 1));
// 	if (!new)
// 		return (NULL);
// 	i = 0;
// 	j = 0;
// 	ft_rm_quote_iterate(s, &new, &i, &j);
// 	ft_add_garbage(state, new);
// 	return (new);
// }

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

int	ft_take_it(t_lexer *l, t_state *s, char *g, int *i, int *j)
{
	ft_create_token(&s->tokens, \
		ft_trim_quotes(ft_substr(g, *j, *i - *j + 1, s), s), T_ARG);
	*j = *i + 1;
	l->take_it = 0;
	l->is_happend = 0;
	l->is_pipe_added = 0;
	return (0);
}

int	ft_merge_args_init(t_token *start_token, int *total_args, t_token **will_change)
{
	t_token	*tmp;

	tmp = start_token;
	while (tmp)
	{
		if (tmp->type == T_ARG)
		{
			if (!(*will_change))
				*will_change = tmp;
			(*total_args)++;
		}
		tmp = tmp->next;
	}
	if (*total_args == 0 || *total_args == 1)
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

void	ft_lexer_bychar_pipe(t_state *s, t_lexer *l, int *i, int *j)
{
	if (l->quote == QUOTE_NONE && l->str[*i] == '|')
	{
		if (*i != 0 && !l->is_pipe_added)
			ft_create_token(&s->tokens, \
				ft_trim_quotes(ft_substr(l->str, *j, *i - *j, s), s), T_ARG);
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
			ft_trim_quotes(ft_substr(l->str, *j, *i - *j + 1, s), s), T_CMD);
		l->is_pipe_added = 0;
		return (2);
	}
	ft_lexer_bychar_pipe(s, l, i, j);
	if (!l->str[*i + 1] && l->quote == QUOTE_NONE && !l->is_pipe_added)
		l->take_it = 1;
	if (l->take_it)
		ft_take_it(l, s, l->str, i, j);
	return (0);
}

int	ft_lexer_bychar(t_state *s, t_lexer *l)
{
	int 	i;
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

int	ft_is_empty(char *s)
{
	int	i;

	i = ft_strlen(s);
	if (i == 0)
		return (1);
	return (0);
}

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
