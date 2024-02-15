/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: egumus <egumus@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/14 23:16:12 by egumus            #+#    #+#             */
/*   Updated: 2024/02/15 03:36:19 by egumus           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_tab_len(char **arr)
{
	int i;
	
	i = 0;
	while (arr[i] != NULL)
		i++;
	return (i);
}

void	ft_free_tab(char **arr)
{
	int i;
	int	size;

	i = -1;
	size = ft_tab_len(arr);
	while (++i < size)
		free(arr[i]);
	free(arr);
	arr = NULL;
}

int	ft_strcmp(const char *s1, const char *s2)
{
	int	i;

	i = 0;
	while (s1[i] && s2[i] && s1[i] == s2[i])
		i++;
	return ((unsigned char)s1[i] - (unsigned char)s2[i]);
}

static int	ft_countwords(char const *str, char c)
{
	int	count;
	int	i;

	i = 0;
	count = 0;
	while (str[i])
	{
		while (str[i] == c)
			i++;
		if (str[i] != c && str[i] != '\0')
			count++;
		while (str[i] != c && str[i] != '\0')
			i++;
	}
	return (count);
}

char	**ft_msplit(char const *s, char c, t_state *state)
{
	int		i;
	int		k;
	int		tab_index;
	char	**tab;

	if (!s)
		return (NULL);
	tab = (char **)malloc(sizeof(char *) * (ft_countwords(s, c) + 1));
	if (!tab)
		return (NULL);
	i = 0;
	tab_index = 0;
	while (s[i])
	{
		while (s[i] == c)
			i++;
		k = i;
		while (s[i] != c && s[i] != '\0')
			i++;
		if (i > k)
			tab[tab_index] = ft_substr(s, k, i - k);
		ft_add_garbage(&state->garbage, tab[tab_index++]);
	}
	tab[tab_index] = NULL;
	return (ft_add_garbage(&state->garbage, tab), tab);
}
