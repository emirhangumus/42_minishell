/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: egumus <egumus@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/18 19:20:09 by egumus            #+#    #+#             */
/*   Updated: 2024/04/01 02:20:29 by egumus           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	ft_count_words(char *s, char c)
{
	int		i;
	int		count;

	i = 0;
	count = 0;
	while (s[i])
	{
		if (s[i] != c)
		{
			count++;
			while (s[i] && s[i] != c)
				i++;
		}
		else
			i++;
	}
	return (count);
}

static int	ft_wordlen(char *s, char c)
{
	int		i;

	i = 0;
	while (s[i] && s[i] != c)
		i++;
	return (i);
}

static int	ft_split_helper(char **arr, char *s, char c)
{
	int		i;
	int		j;
	int		k;

	i = 0;
	j = 0;
	while (s[i])
	{
		if (s[i] != c)
		{
			k = 0;
			arr[j] = (char *)malloc(sizeof(char) * (ft_wordlen(&s[i], c) + 1));
			if (!arr[j])
				return (ft_free_prevs(arr, j - 1), 1);
			while (s[i] && s[i] != c)
				arr[j][k++] = s[i++];
			arr[j++][k] = '\0';
		}
		else
			i++;
	}
	arr[j] = NULL;
	return (0);
}

char	**ft_split(char *s, char c, t_state *state)
{
	char	**arr;

	arr = (char **)malloc(sizeof(char *) * (ft_count_words(s, c) + 1));
	if (!arr)
		return (NULL);
	if (ft_split_helper(arr, s, c))
		return (NULL);
	ft_addarr_garbage(state, (void **)arr);
	return (arr);
}
