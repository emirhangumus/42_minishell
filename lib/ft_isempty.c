/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isempty.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: egumus <egumus@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/01 14:46:49 by egumus            #+#    #+#             */
/*   Updated: 2024/03/01 14:47:01 by egumus           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	ft_is_empty(char *s)
{
	int	i;

	i = ft_strlen(s);
	if (i == 0)
		return (1);
	return (0);
}