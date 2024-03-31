/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_remove_char_by_index.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: egumus <egumus@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/31 22:16:05 by egumus            #+#    #+#             */
/*   Updated: 2024/03/31 22:33:54 by egumus           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

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
	ft_add_garbage(s, new);
	*str = new;
}
