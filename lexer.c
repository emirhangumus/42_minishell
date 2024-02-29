/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: egumus <egumus@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/20 19:32:25 by egumus            #+#    #+#             */
/*   Updated: 2024/02/29 18:32:13 by egumus           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_add_token(t_state *s, char *token, int type)
{
	t_token	*new;
	t_token	*tmp;

	new = (t_token *)malloc(sizeof(t_token));
	if (!new)
		return ;
	new->value = token;
	new->type = type;
	new->next = NULL;
	if (!s->tokens)
		s->tokens = new;
	else
	{
		tmp = s->tokens;
		while (tmp->next)
			tmp = tmp->next;
		tmp->next = new;
	}
}

void	ft_create_token(t_token **token, char *value, int type)
{
	t_token	*new;
	t_token	*tmp;

	new = (t_token *)malloc(sizeof(t_token));
	if (!new)
		return ;
	new->value = value;
	new->type = type;
	new->next = NULL;
	if (!*token)
		*token = new;
	else
	{
		tmp = *token;
		while (tmp->next)
			tmp = tmp->next;
		tmp->next = new;
	}
}

t_token *ft_get_last_token(t_token *token)
{
	t_token *tmp;

	tmp = token;
	while (tmp->next)
		tmp = tmp->next;
	return (tmp);
}

void	ft_remove_tokens(t_token **token, int (*f)(void *)) // void *(*f)(void *)
{
	t_token	*tmp;
	t_token	*prev;

	tmp = *token;
	prev = NULL;
	while (tmp)
	{
		if (f(tmp->value))
		{
			if (prev)
				prev->next = tmp->next;
			else
				*token = tmp->next;
			free(tmp);
			tmp = NULL;
			break ;
		}
		prev = tmp;
		tmp = tmp->next;
	}
}

char	*ft_remove_quotes(char *s, t_state *state)
{
	int		i;
	int		j;
	int		quote_type;
	char	*new;

	i = 0;
	j = 0;
	quote_type = QUOTE_NONE;
	while (s[i] != '\0')
	{
		if (s[i] == '\'' || s[i] == '\"')
		{
			if (quote_type == QUOTE_NONE)
			{
				quote_type = s[i];
				i++;
				continue ;
			}
			else if (quote_type == s[i])
			{
				quote_type = QUOTE_NONE;
				i++;
				continue ;
			}
			else {
				
			}
		}
		i++;
		j++;
	}
	new = (char *)malloc(sizeof(char) * (j + 1));
	if (!new)
		return (NULL);
	i = 0;
	j = 0;
	quote_type = QUOTE_NONE;
	while (s[i])
	{
		if (s[i] == '\'' || s[i] == '\"')
		{
			if (quote_type == QUOTE_NONE)
			{
				quote_type = s[i];
				i++;
				continue ;
			}
			else if (quote_type == s[i])
			{
				quote_type = QUOTE_NONE;
				i++;
				continue ;
			}
						else {
				
			}
		}
		new[j] = s[i];
		i++;
		j++;
	}
	new[j] = '\0';
	ft_add_garbage(state, new);
	return (new);
}

void	ft_print_tokens(t_token *token)
{
	while (token)
	{
		printf("type: %d, value: %s\n", token->type, token->value);
		token = token->next;
	}
}

void	ft_free_tokens(t_token *token)
{
	t_token	*tmp;

	while (token)
	{
		tmp = token;
		token = token->next;
		free(tmp);
	}
}

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
	ft_create_token(&s->tokens, ft_trim_quotes(ft_substr(g, *j, *i - *j + 1, s), s), T_ARG);
	*j = *i + 1;
	l->take_it = 0;
	l->is_happend = 0;
	l->is_pipe_added = 0;
	return (0);
}

// void	ft_remove_args(t_token *start_token, t_state *s)
// {
// 	t_token *tmp;
// 	char *str;
	
// 	tmp = start_token;
// }

void	ft_merge_args(t_token *start_token, t_state *s)
{
	int	total_args;
	t_token *tmp;
	t_token *will_change;
	char *str;
	
	tmp = start_token;
	will_change = NULL;
	total_args = 0;
	str = NULL;
	while (tmp)
	{
		if (tmp->type == T_ARG)
		{
			if (will_change == NULL)
				will_change = tmp;
			total_args++;
		}
		tmp = tmp->next;
	}
	if (total_args == 0 || total_args == 1)
		return ;
	printf("total_args: %d\n", total_args);
	tmp = start_token;
	while (tmp)
	{
		if (tmp->type != T_ARG)
		{
			tmp = tmp->next;
			continue ;
		}
		if (tmp->next && tmp->next->type == T_ARG)
		{
			str = ft_strjoin(tmp->value, ft_strdup(tmp->next->value, s), s);
			tmp->next = tmp->next->next;
		}
		tmp = tmp->next;
	}
	will_change->value = str;
}

int	ft_lexer_bychar(t_state *s, t_lexer *l, char *str)
{
	int 	i;
	int 	j;
	t_token	*tmp;

	i = 0;
	j = 0;
	tmp = ft_get_last_token(s->tokens);
	l->is_happend = 0;
	l->take_it = 0;
	while (str[i])
	{
		// handle | < < > >>
		if (ft_toggle_quote(l, str[i]) == 1)
		{
			j = i;
			i++;
			continue ;
		}
		if (l->quote == QUOTE_NONE && l->is_pipe_added)
		{
			j = i;
			// go to end
			while (str[i])
				i++;
			ft_create_token(&s->tokens, ft_trim_quotes(ft_substr(str, j, i - j + 1, s), s), T_CMD);
			l->is_pipe_added = 0;
			break ;
		}
		if (l->quote == QUOTE_NONE && str[i] == '|')
		{
			if (i != 0 && !l->is_pipe_added)
				ft_create_token(&s->tokens, ft_trim_quotes(ft_substr(str, j, i - j, s), s), T_CMD);
			ft_create_token(&s->tokens, ft_strdup("|", s), T_PIPE);
			l->is_pipe_added = 1;
			j = i + 1;
		}	
		if (!str[i + 1] && l->quote == QUOTE_NONE && !l->is_pipe_added)
			l->take_it = 1;
		if (l->take_it)
			ft_take_it(l, s, str, &i, &j);
		i++;
	}
	if (l->quote != QUOTE_NONE)
		return (1);
	ft_merge_args(tmp, s);
	return (0);
}

int	ft_is_empty(char *s)
{
	int	i;

	i = ft_strlen(s);
	if (i == 0)
		return (1);
	return (0);
}

int	ft_lexer(t_state *s)
{
	t_lexer	*l;

	l = (t_lexer *)malloc(sizeof(t_lexer));
	if (!l)
		return (1);
	l->i = 0;
	l->is_happend = 0;
	l->is_pipe_added = 0;
	l->take_it = 0;
	l->quote = QUOTE_NONE;
	l->sp = ft_quote_split(s->cmd, ' ', s); //kaliteli hale getirilmeli kardeşim
	if (!l->sp)
		return (1);
	while (l->sp[l->i])
	{
		if (l->i == 0 || l->is_pipe_added)
		{
			ft_create_token(&s->tokens, ft_trim_quotes(l->sp[l->i], s), T_CMD);
			if (l->is_pipe_added && l->i != 0)
				l->is_pipe_added = 0;
			l->i++;
			continue ;
		}
		if (ft_lexer_bychar(s, l, l->sp[l->i]))
		{
			printf("Error: quote not closed\n");
			return (1);
		}
		l->i++;
	}
	ft_remove_tokens(&s->tokens, (int (*)(void *))ft_is_empty);
	ft_print_tokens(s->tokens);
	ft_free_tokens(s->tokens);
	s->tokens = NULL;
	return (0);
}

/**
echo "asd"|cat -e

echo -> command
"asd" -> arg
| -> pipe
cat -> command
-e -> arg



echo "asd" | cat -e
echo -> command
"asd" -> arg
| -> pipe
cat -> command
-e -> arg



echo "asd|cat -e"

echo -> command
"asd|cat -e" -> arg

ls -l | cat -e

ls -> command
-l -> arg
| -> pipe
cat -> command
-e -> arg

*/