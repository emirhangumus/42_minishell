/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: egumus <egumus@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/03 16:34:36 by egumus            #+#    #+#             */
/*   Updated: 2024/03/31 22:31:01 by egumus           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	ifsendsigquit(int signal)
{
	(void)signal;
	rl_redisplay();
	return ;
}

static void	ifsendsigint(int signal)
{
	(void)signal;
	write(1, "\033[A", 4);
	ioctl(0, TIOCSTI, "\n");
}

void	ft_signals(void)
{
	signal(SIGINT, ifsendsigint);
	signal(SIGQUIT, ifsendsigquit);
}
