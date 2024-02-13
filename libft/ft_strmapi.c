/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strmapi.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: egumus <egumus@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/04 21:45:42 by egumus            #+#    #+#             */
/*   Updated: 2023/10/04 22:16:09 by egumus           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strmapi(char const *s, char (*f)(unsigned int, char))
{
	unsigned int	i;
	unsigned int	len;
	char			*str;

	if (!s)
		return (NULL);
	i = 0;
	len = (unsigned int)ft_strlen(s);
	str = (char *)malloc(sizeof(char) * ((int)len + 1));
	if (!str)
		return (NULL);
	while (i < len)
	{
		str[i] = f(i, s[(int)i]);
		i++;
	}
	str[i] = '\0';
	return ((char *)str);
}
