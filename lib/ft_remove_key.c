/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_remove_key.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: egumus <egumus@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/31 22:17:36 by egumus            #+#    #+#             */
/*   Updated: 2024/03/31 22:17:39 by egumus           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	ft_remove_key(char **str, int start_index, int end_index, t_state *s)
{
	while (end_index != start_index)
	{
		ft_remove_char_by_index(str, start_index, s);
		end_index--;
	}
}
