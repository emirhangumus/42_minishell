/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_is_redirect.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: egumus <egumus@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/31 22:35:58 by egumus            #+#    #+#             */
/*   Updated: 2024/04/01 05:00:56 by egumus           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	ft_is_redirect_helper(char *original)
{
	int	quote;
	int	i;

	i = 0;
	quote = QUOTE_NONE;
	while (original[i])
	{
		if (quote == QUOTE_NONE && (original[i] == '\''
				|| original[i] == '\"'))
			quote = original[i];
		else if (quote == original[i])
			quote = QUOTE_NONE;
		if (quote == QUOTE_NONE && ft_strncmp(original + i, "<<", 2) == 0)
			return (2);
		if (quote == QUOTE_NONE && ft_strncmp(original + i, ">>", 2) == 0)
			return (2);
		if (quote == QUOTE_NONE && ft_strncmp(original + i, ">", 1) == 0)
			return (1);
		if (quote == QUOTE_NONE && ft_strncmp(original + i, "<", 1) == 0)
			return (1);
		i++;
	}
	return (SUCCESS);
}

int	ft_is_redirect(char *str, char *original)
{
	if (original)
		return (ft_is_redirect_helper(original));
	else
	{
		if (ft_strncmp(str, "<<", 2) == 0)
			return (2);
		if (ft_strncmp(str, ">>", 2) == 0)
			return (2);
		if (ft_strncmp(str, ">", 1) == 0)
			return (1);
		if (ft_strncmp(str, "<", 1) == 0)
			return (1);
		return (SUCCESS);
	}
}
