/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: burkaya <burkaya@student.42istanbul.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/18 19:18:08 by egumus            #+#    #+#             */
/*   Updated: 2024/03/01 11:04:53 by burkaya          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*ft_strdup(char *str, t_state *s)
{
	char	*new;
	int		i;

	i = 0;
	new = (char *)malloc(sizeof(char) * (ft_strlen(str) + 1));
	if (!new)
		return (NULL);
	if (s != NULL)
		ft_add_garbage(s, new);
	while (str[i])
	{
		new[i] = str[i];
		i++;
	}
	new[i] = '\0';
	return (new);
}
