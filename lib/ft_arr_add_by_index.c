/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_arr_add_by_index.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: burkaya <burkaya@student.42istanbul.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/30 01:11:42 by burkaya           #+#    #+#             */
/*   Updated: 2024/03/30 14:40:31 by burkaya          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	ft_arr_add_by_index(char ***arr, char *str, int index, t_state *s)
{
	int		len;
	char	**new_arr;
	int		i;
	int		j;

	len = ft_arr_len(*arr);
	new_arr = malloc(sizeof(char *) * (len + 2));
	if (new_arr == NULL)
		return ;
	i = 0;
	j = 0;
	while (i <= len)
	{
		if (i == index)
		{
			new_arr[j] = ft_strdup(str, s);
			if (new_arr[j] == NULL)
				return ;
			j++;
		}
		if ((*arr)[i] != NULL)
		{
			new_arr[j] = ft_strdup((*arr)[i], s);
			if (new_arr[j] == NULL)
				return ;
			j++;
		}
		i++;
	}
	new_arr[j] = NULL;
	if (s)
		ft_add_garbage(s, new_arr);
	*arr = new_arr;
}
