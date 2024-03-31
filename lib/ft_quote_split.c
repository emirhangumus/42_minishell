/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_quote_split.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: egumus <egumus@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/20 21:23:06 by egumus            #+#    #+#             */
/*   Updated: 2024/04/01 00:16:34 by egumus           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	ft_count_words_with_quotes(char *s, char *c)
{
	int		count;
	int		seen_quote_type;
	int		toggle;

	count = 0;
	toggle = 0;
	seen_quote_type = 0;
	while (*s)
	{
		if (seen_quote_type == QUOTE_NONE && (*s == '\'' || *s == '\"'))
			seen_quote_type = (int)*s;
		else if (seen_quote_type == *s)
			seen_quote_type = QUOTE_NONE;
		if (seen_quote_type == QUOTE_NONE && ft_strncmp(s, c, ft_strlen(c)) == 0)
		{
			count++;
			toggle = 0;
		}
		else
			toggle = 1;
		s++;
	}
	if (toggle == 1)
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
	if (*seen_quote_type == QUOTE_NONE && (*s == '\'' || *s == '\"'))
		*seen_quote_type = (int)*s;
	else if (*seen_quote_type == (int)*s)
		*seen_quote_type = QUOTE_NONE;
	(*i)++;
}

char	**ft_quote_split(char *s, char *c, t_state *state)
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
		k = i;
		while (s[i] && (seen_quote_type != QUOTE_NONE || ft_strncmp(&s[i], c, ft_strlen(c)) != 0))
			ft_set_quote_type(&s[i], &i, &seen_quote_type);
		if (i > k)
		{
			if (ft_strlen(c) == ft_strlen(s) && ft_strncmp(s, c, ft_strlen(c)) == 0)
			{
				result[j] = ft_strdup("", state);
				if (!result[j])
					return (ft_free(result, j));
				break ;
			}
			result[j++] = ft_substr(s, k, i - k, state);
			if (!result[j - 1])
				return (ft_free(result, j - 1));
			if (s[i])
				i += ft_strlen(c) - 1;
		}
		if (!s[i])
			break ;
		i++;
	}
	result[j] = NULL;
	ft_add_garbage(state, result);
	return (result);
}
