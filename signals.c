/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: egumus <egumus@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/03 16:34:36 by egumus            #+#    #+#             */
/*   Updated: 2024/04/03 01:34:31 by egumus           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	coix(int sig)
{
	(void)sig;
	rl_on_new_line();
	printf("\033[K");
	rl_redisplay();
	g_qsignal = 0;
}

void	ctrl_c(int sig)
{
	(void)sig;
	if (!g_qsignal)
	{
		printf("\n");
		rl_replace_line("", 0);
		rl_on_new_line();
		rl_redisplay();
	}
	else
		printf("\n");
}

static void	tcsa(void)
{
	struct termios	termios_p;

	if (tcgetattr(0, &termios_p) != 0)
		perror("Minishell: tcgetattr");
	termios_p.c_lflag &= ~ECHOCTL;
	if (tcsetattr(0, 0, &termios_p) != 0)
		perror("Minishell: tcsetattr");
}

void	ft_signals(void)
{
	tcsa();
	signal(SIGINT, ctrl_c);
	signal(SIGQUIT, coix);
	return ;
}
