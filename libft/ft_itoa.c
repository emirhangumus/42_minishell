/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: egumus <egumus@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/04 17:23:21 by egumus            #+#    #+#             */
/*   Updated: 2023/10/05 03:57:34 by egumus           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	count_digit_and_is_negative(long n)
{
	int		count;

	if (n == 0)
		return (1);
	count = 0;
	if (n < 0)
	{
		count++;
		n = n * -1;
	}
	while (n / 10 != 0)
	{
		count++;
		n = n / 10;
	}
	count++;
	return (count);
}

static void	make_job(char *str, size_t nb, int *level)
{
	if (nb > 9)
	{
		make_job(str, nb / 10, level);
		make_job(str, nb % 10, level);
	}
	else
	{
		str[*level] = nb + '0';
		(*level)++;
	}
}

char	*ft_itoa(int n)
{
	char	*str;
	int		level;
	long	nb;
	size_t	i;

	nb = n;
	str = (char *)malloc(sizeof(char) * (count_digit_and_is_negative(nb) + 1));
	if (!str)
		return (NULL);
	i = 0;
	level = 0;
	if (n < 0)
	{
		str[0] = '-';
		nb = nb * -1;
		level++;
	}
	make_job(str, (size_t)nb, &level);
	str[level] = '\0';
	return (str);
}
