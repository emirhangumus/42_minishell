/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokens.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: burkaya <burkaya@student.42istanbul.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/01 14:28:31 by egumus            #+#    #+#             */
/*   Updated: 2024/04/01 06:40:44 by burkaya          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_add_token(t_state *s, char *token, int type, int index)
{
	t_token	*new;

	if (!s->tokens)
		return ;
	new = ft_create_token(token, type);
	if (!new)
		return ;
	if (!s->tokens[index])
		s->tokens[index] = new;
	else
	{
		new->prev = ft_get_last_token(s->tokens[index]);
		ft_get_last_token(s->tokens[index])->next = new;
	}
}

t_token	*ft_create_token(char *value, int type)
{
	t_token	*new;

	new = (t_token *)malloc(sizeof(t_token));
	if (!new)
		return (NULL);
	new->value = ft_strdup(value, NULL);
	new->type = type;
	new->next = NULL;
	new->prev = NULL;
	return (new);
}

t_token	*ft_get_last_token(t_token *token)
{
	t_token	*tmp;

	tmp = token;
	while (tmp->next)
		tmp = tmp->next;
	return (tmp);
}

void	ft_free_tokens(t_token **token)
{
	t_token	**tmp;
	t_token	*next;
	t_token	*tmp2;
	int		i;

	if (!token || !*token)
		return ;
	tmp = token;
	i = 0;
	while (tmp[i])
	{
		next = tmp[i];
		while (next)
		{
			tmp2 = next;
			next = next->next;
			free(tmp2->value);
			free(tmp2);
		}
		i++;
	}
}

void	ft_init_prev_tokens(t_token **tokens)
{
	t_token	**tmp;
	t_token	*next;
	int		i;

	tmp = tokens;
	i = 0;
	while (tmp[i])
	{
		next = tmp[i];
		while (next->next)
		{
			next->next->prev = next;
			next = next->next;
		}
		i++;
	}
}
