/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_joinstr_index.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: egumus <egumus@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/31 22:16:40 by egumus            #+#    #+#             */
/*   Updated: 2024/04/01 02:21:22 by egumus           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	ft_joinstr_index_creation(char *s1, char *s2, \
	int start_index, char *new)
{
	int	i;
	int	j;
	int	k;

	i = 0;
	j = 0;
	while (s1[i] && i < start_index)
	{
		new[i] = s1[i];
		i++;
	}
	k = i;
	while (s2[j])
	{
		new[i] = s2[j];
		i++;
		j++;
	}
	while (s1[k])
	{
		new[i] = s1[k];
		i++;
		k++;
	}
	new[i] = '\0';
}

char	*ft_joinstr_index(char *s1, char *s2, int start_index, t_state *s)
{
	char	*new;
	int		i;
	int		j;

	i = 0;
	j = 0;
	new = (char *)malloc(ft_strlen(s1) + ft_strlen(s2) + 1);
	if (!new)
		return (NULL);
	ft_joinstr_index_creation(s1, s2, start_index, new);
	ft_add_garbage(s, new);
	return (new);
}
