/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_arr_remove_by_index.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: burkaya <burkaya@student.42istanbul.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/30 01:27:13 by burkaya           #+#    #+#             */
/*   Updated: 2024/03/30 14:40:25 by burkaya          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	ft_arr_remove_by_index(char ***arr, int index, t_state *s)
{
	char	**new;
	int		i;
	int		j;

	i = 0;
	j = 0;
	if (!arr || !*arr)
		return ;
	new = (char **)malloc(sizeof(char *) * (ft_arr_len(*arr)));
	if (!new)
		return ;
	while ((*arr)[i])
	{
		if (i != index)
		{
			new[j] = (*arr)[i];
			j++;
		}
		i++;
	}
	new[j] = NULL;
	if (s)
		ft_add_garbage(s, new);
	*arr = new;
}
