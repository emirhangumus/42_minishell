/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: burkaya <burkaya@student.42istanbul.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/20 19:36:17 by egumus            #+#    #+#             */
/*   Updated: 2024/03/01 11:01:13 by burkaya          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*ft_strchr(const char *s, int c)
{
	unsigned char	*ptr;

	ptr = (unsigned char *)s;
	while (*ptr)
	{
		if (*ptr == (unsigned char)c)
			return ((char *)ptr);
		ptr++;
	}
	if (*ptr == (unsigned char)c)
		return ((char *)ptr);
	return (NULL);
}
