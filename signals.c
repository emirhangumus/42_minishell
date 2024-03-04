/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: burkaya <burkaya@student.42istanbul.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/03 16:34:36 by egumus            #+#    #+#             */
/*   Updated: 2024/03/04 04:05:17 by burkaya          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern int	g_sig;

void	ft_sigquit(int sig)
{
	(void)sig;
	if (g_sig == 1)
	{
		write(1, "\033[2D", 4);
		write(1, "  ", 2);
		write(1, "\033[2D", 4);
		ioctl(0, TIOCSTI);
		g_sig = 0;
	}
	g_sig = 1;
}

void	ft_sigint(int sig)
{
	(void)sig;
	write(1, "\033[A", 3);
	ioctl(0, TIOCSTI, "\n");
	g_sig = 1;
}

void	ft_signals(void)
{
	signal(SIGINT, ft_sigint);
	signal(SIGQUIT, ft_sigquit);
}
