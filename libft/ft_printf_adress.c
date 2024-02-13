/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_adress.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: egumus <egumus@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/14 02:58:20 by egumus            #+#    #+#             */
/*   Updated: 2023/10/14 02:58:52 by egumus           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	ft_to_hex_pf(size_t n, int i, char *set)
{
	if (n >= 16)
	{
		i = ft_to_hex_pf(n / 16, i, set);
		i = ft_to_hex_pf(n % 16, i, set);
	}
	else
		i = ft_putchar_pf(set[n], i);
	return (i);
}

int	ft_putadress(void *p, int i)
{
	size_t	ptr;

	if (p == NULL)
	{
		i = ft_putstr_pf("0x0", i);
	}
	else
	{
		ptr = (size_t)p;
		i = ft_putstr_pf("0x", i);
		i = ft_to_hex_pf(ptr, i, "0123456789abcdef");
	}
	return (i);
}
