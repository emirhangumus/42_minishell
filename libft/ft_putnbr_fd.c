/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: egumus <egumus@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/04 22:28:46 by egumus            #+#    #+#             */
/*   Updated: 2023/10/05 03:56:23 by egumus           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static void	make_job(long nb, int *level, int fd)
{
	if (nb < 0)
	{
		ft_putchar_fd('-', fd);
		nb *= -1;
	}
	if (nb > 9)
	{
		make_job(nb / 10, level, fd);
		make_job(nb % 10, level, fd);
	}
	else
	{
		ft_putchar_fd(nb + '0', fd);
		(*level)++;
	}
}

void	ft_putnbr_fd(int n, int fd)
{
	int		level;
	long	nb;

	level = 0;
	nb = n;
	make_job(nb, &level, fd);
}
