/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_invalid.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: burkaya <burkaya@student.42istanbul.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/31 22:43:09 by egumus            #+#    #+#             */
/*   Updated: 2024/04/01 06:38:40 by burkaya          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	ft_v_invalid_redirect(t_token *tokens)
{
	t_token	*tmp;
	int		i;
	int		err;

	i = 0;
	err = 0;
	tmp = tokens;
	while (tmp)
	{
		// redirections cant be next to eact other
		if (ft_is_redirect(tmp->value, NULL))
		{
			if (tmp->prev && ft_is_redirect(tmp->prev->value, NULL))
			{
				printf("this: %s\n", tmp->value);
				printf("prev: %s\n", tmp->prev->value);
				return (ERR_UNEXPECTED_TOKEN);
			}
			if (tmp->next && ft_is_redirect(tmp->next->value, NULL))
			{
				printf("next: %s\n", tmp->next->value);
				return (ERR_UNEXPECTED_TOKEN);
			}
		}
		tmp = tmp->next;
	}
	return (err);
}

int	ft_lexer_validate(t_state *s)
{
	t_token	**tmp;
	int		i;
	int		err;

	i = 0;
	err = 0;
	tmp = s->tokens;
	while (tmp[i])
	{
		err = ft_v_invalid_redirect(tmp[i]);
		if (err)
			return (err);
		i++;
	}
	return (err);
}

int	ft_check_invalid_pipes(char *cmd, int si)
{
	int	quote;
	int	err;

	quote = QUOTE_NONE;
	err = 0;
	while (cmd[si])
	{
		if (quote == QUOTE_NONE && (cmd[si] == '\'' || cmd[si] == '\"'))
			quote = cmd[si];
		else if (quote == cmd[si])
			quote = QUOTE_NONE;
		if (quote == QUOTE_NONE && cmd[si] == '|')
		{
			if (si == 0)
				err = ERR_UNEXPECTED_TOKEN;
			else if (cmd[si + 1] == '\0')
				err = ERR_UNEXPECTED_TOKEN;
			else if (cmd[si + 1] == '|')
				err = ERR_UNEXPECTED_TOKEN;
		}
		if (err)
			return (err);
		si++;
	}
	return (err);
}
