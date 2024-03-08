/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_quote_split.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: egumus <egumus@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/20 21:23:06 by egumus            #+#    #+#             */
/*   Updated: 2024/03/08 08:21:39 by egumus           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	ft_count_words_with_quotes(char *s, char c)
{
	int		count;
	int		seen_quote_type;
	int		toggle;

	count = 0;
	toggle = 0;
	seen_quote_type = 0;
	while (*s)
	{
		if (*s == '\'' || *s == '\"')
		{
			if (seen_quote_type == QUOTE_NONE)
				seen_quote_type = *s;
			else if (seen_quote_type == QUOTE_SINGLE && *s == '\'')
				seen_quote_type = QUOTE_NONE;
			else if (seen_quote_type == QUOTE_DOUBLE && *s == '\"')
				seen_quote_type = QUOTE_NONE;
		}
		if (*s == c && seen_quote_type == QUOTE_NONE)
		{
			count++;
			toggle = 0;
		}
		else
			toggle = 1;
		s++;
	}
	if (*s != c || toggle)
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
		if (*seen_quote_type == QUOTE_NONE)
			*seen_quote_type = s[*i];
		else if (*seen_quote_type == QUOTE_SINGLE && s[*i] == '\'')
			*seen_quote_type = QUOTE_NONE;
		else if (*seen_quote_type == QUOTE_DOUBLE && s[*i] == '\"')
			*seen_quote_type = QUOTE_NONE;
	}
	(*i)++;
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
	while (s[i] && s[i] == c)
		i++;
	while (s[i])
	{
		k = i;
		while (s[i] && (seen_quote_type != QUOTE_NONE || s[i] != c))
			ft_set_quote_type(s, &i, &seen_quote_type);
		if (i > k)
			result[j++] = ft_substr(s, k, i - k, state);
		if (!result[j - 1])
			return (ft_free(result, j - 1));
		if (!s[i])
			break ;
		i++;
	}
	result[j] = NULL;
	if (state)
		ft_add_garbage(state, result);
	return (result);
}
