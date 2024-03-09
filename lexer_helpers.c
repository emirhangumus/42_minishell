/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_helpers.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: burkaya <burkaya@student.42istanbul.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/09 16:52:26 by burkaya           #+#    #+#             */
/*   Updated: 2024/03/09 16:53:01 by burkaya          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_remove_char_by_index(char **str, int index, t_state *s)
{
	char	*new;
	int		i;
	int		j;

	i = 0;
	j = 0;
	if (!str || !*str)
		return ;
	if (index < 0 || index > (int)ft_strlen(*str))
		return ;
	new = (char *)malloc(ft_strlen(*str));
	if (!new)
		return ;
	while ((*str)[i])
	{
		if (i != index)
		{
			new[j] = (*str)[i];
			j++;
		}
		i++;
	}
	new[j] = '\0';
	if (s)
		ft_add_garbage(s, new);
	*str = new;
}

char	*ft_joinstr_index(char *s1, char *s2, int start_index, t_state *s)
{
	char	*new;
	int		i;
	int		j;
	int		k;
	// merge two strings s1 and s2 starting from index start_index

	i = 0;
	j = 0;
	new = (char *)malloc(ft_strlen(s1) + ft_strlen(s2) + 1);
	if (!new)
		return (NULL);
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
	if (s)
		ft_add_garbage(s, new);
	return (new);
}
