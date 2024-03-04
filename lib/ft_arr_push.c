/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_arr_push.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: burkaya <burkaya@student.42istanbul.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/04 04:17:36 by burkaya           #+#    #+#             */
/*   Updated: 2024/03/04 04:17:44 by burkaya          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	ft_arr_push(char ***arr, char *str)
{
	int		len;
	char	**new_arr;
	int		i;

	len = ft_arr_len(*arr);
	new_arr = malloc(sizeof(char *) * (len + 2));
	if (new_arr == NULL)
		return ;
	i = 0;
	while (i < len)
	{
		new_arr[i] = (*arr)[i];
		i++;
	}
	new_arr[i] = str;
	new_arr[i + 1] = NULL;
	free(*arr);
	*arr = new_arr;
}
