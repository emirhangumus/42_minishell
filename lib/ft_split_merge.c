/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split_merge.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: egumus <egumus@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/31 22:25:58 by egumus            #+#    #+#             */
/*   Updated: 2024/03/31 22:30:04 by egumus           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static char	**ft_split_merge_helper(t_state *s, int count, char **split)
{
	int		i;
	int		j;
	int		k;
	char	**new_split;
	char	**temp;

	i = -1;
	j = 0;
	k = 0;
	new_split = (char **)malloc(sizeof(char *) * (count + 1));
	if (!new_split)
		return (NULL);
	while (split[++i])
	{
		temp = ft_quote_split(split[i], " ", s);
		k = 0;
		while (temp[k])
		{
			new_split[j] = ft_strdup(temp[k], s);
			j++;
			k++;
		}
	}
	new_split[j] = NULL;
	return (new_split);
}

char	**ft_split_merge(char **split, t_state *s)
{
	int		i;
	char	**temp;
	int		count;

	i = 0;
	count = 0;
	temp = NULL;
	while (split[i])
	{
		temp = ft_quote_split(split[i], " ", s);
		count += ft_arr_len(temp);
		i++;
	}
	temp = ft_split_merge_helper(s, count, split);
	ft_add_garbage(s, temp);
	return (temp);
}
