/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_quote_split.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: burkaya <burkaya@student.42istanbul.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/20 21:23:06 by egumus            #+#    #+#             */
/*   Updated: 2024/03/01 11:07:12 by burkaya          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	ft_count_words_with_quotes(char *s, char c)
{
	int		count;
	int		seen_quote_type;

	count = 0;
	seen_quote_type = 0;
	while (*s)
	{
		if (*s == '\'' || *s == '\"')
		{
			if (seen_quote_type == QUOTE_NONE)
				seen_quote_type = (*s == '\'') ? QUOTE_ONE : QUOTE_TWO;
			else if (seen_quote_type == QUOTE_ONE && *s == '\'')
				seen_quote_type = QUOTE_NONE;
			else if (seen_quote_type == QUOTE_TWO && *s == '\"')
				seen_quote_type = QUOTE_NONE;
			s++;
		}
		if (*s == c && seen_quote_type == QUOTE_NONE)
			count++;
		s++;
	}
	if (*(s - 1) != c)
		count++;
	return (count);
}

static char	**ft_free(char **arr, int i)
{
	while (i >= 0)
	{
		free(arr[i]);
		i--;
	}
	free(arr);
	return (NULL);
}

static	void	ft_set_quote_type(char *s, int *i, int *seen_quote_type)
{
	if (s[*i] == '\'' || s[*i] == '\"')
	{
		if (s[*i] == '\'')
			*seen_quote_type = QUOTE_ONE;
		else
			*seen_quote_type = QUOTE_TWO;
		(*i)++;
		while (s[*i] && s[*i] != *seen_quote_type)
			(*i)++;
		if (s[*i] == *seen_quote_type)
			(*i)++;
	}
}

char	**ft_quote_split(char *s, char c, t_state *state)
{
	int		seen_quote_type;
	char	**result;
	int		i;
	int		j;
	int		k;

	seen_quote_type = 0;
	i = 0;
	j = 0;
	k = 0;
	result = (char **)malloc(sizeof(char *) * \
		(ft_count_words_with_quotes(s, c) + 1));
	if (!result)
		return (NULL);
	while (s[i])
	{
		while (s[i] && s[i] == c)
			i++;
		k = i;
		ft_set_quote_type(s, &i, &seen_quote_type);
		while (s[i] != c && s[i] != '\0')
			i++;
		if (i > k)
			result[j++] = ft_substr(s, k, i - k, state);
		if (!result[j - 1])
			return (ft_free(result, j - 1));
		if (!s[i])
			break ;
		i++;
	}
	result[j] = NULL;
	return (result);
}
