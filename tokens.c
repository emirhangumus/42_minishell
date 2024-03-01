/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokens.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: egumus <egumus@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/01 14:28:31 by egumus            #+#    #+#             */
/*   Updated: 2024/03/01 14:52:35 by egumus           ###   ########.fr       */
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

t_token	*ft_get_last_token(t_token *token)
{
	t_token	*tmp;

	tmp = token;
	while (tmp->next)
		tmp = tmp->next;
	return (tmp);
}

void	ft_remove_tokens(t_token **token, int (*f)(void *))
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
