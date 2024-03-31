/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_invalid.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: egumus <egumus@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/31 22:43:09 by egumus            #+#    #+#             */
/*   Updated: 2024/03/31 22:53:44 by egumus           ###   ########.fr       */
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
		if (tmp->type == T_LREDIR && (!tmp->next || tmp->next->type != T_ARG))
			err = ERR_UNCOMPLETED_REDIRECT;
		if (tmp->type == T_RREDIR && (!tmp->next || tmp->next->type != T_ARG))
			err = ERR_UNCOMPLETED_REDIRECT;
		if (tmp->type == T_LAPPEND && (!tmp->next || tmp->next->type != T_ARG))
			err = ERR_UNCOMPLETED_REDIRECT;
		if (tmp->type == T_RAPPEND && (!tmp->next || tmp->next->type != T_ARG))
			err = ERR_UNCOMPLETED_REDIRECT;
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

int	ft_check_invalid_pipes(char *cmd, int start_index)
{
	int	quote;
	int	err;

	quote = QUOTE_NONE;
	err = 0;
	while (cmd[start_index])
	{
		if (quote == QUOTE_NONE && (cmd[start_index] == '\'' || cmd[start_index] == '\"'))
			quote = cmd[start_index];
		else if (quote == cmd[start_index])
			quote = QUOTE_NONE;
		if (quote == QUOTE_NONE && cmd[start_index] == '|')
		{
			if (start_index == 0)
				err = ERR_UNEXPECTED_TOKEN;
			else if (cmd[start_index + 1] == '\0')
				err = ERR_UNEXPECTED_TOKEN;
			else if (cmd[start_index + 1] == '|')
				err = ERR_UNEXPECTED_TOKEN;
		}
		if (err)
			return (err);
		start_index++;
	}
	return (err);
}
