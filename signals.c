/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: burkaya <burkaya@student.42istanbul.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/03 16:34:36 by egumus            #+#    #+#             */
/*   Updated: 2024/03/16 15:48:35 by burkaya          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"



void    ifsendsigquit(int signal)
{
    (void)signal;
	rl_redisplay();
    return ;
}

void    ifsendsigint(int signal)
{
	(void)signal;
    write(1, "\033[A", 4);
    ioctl(0, TIOCSTI, "\n");
}

void    ft_signals(void)
{
    signal(SIGINT, ifsendsigint);
    signal(SIGQUIT, ifsendsigquit);
}
