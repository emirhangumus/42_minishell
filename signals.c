/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: burkaya <burkaya@student.42istanbul.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/03 16:34:36 by egumus            #+#    #+#             */
/*   Updated: 2024/04/01 16:03:41 by burkaya          ###   ########.fr       */
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

void	tcsa(void)
{
	struct termios	term1;

	if (tcgetattr(STDIN_FILENO, &term1) != 0)
		exit((perror("error"), -1));
	else
	{
		term1.c_cc[VQUIT] = _POSIX_VDISABLE;
		term1.c_lflag |= ECHOE | ICANON;
		if (tcsetattr(STDIN_FILENO, TCSANOW, &term1) != 0)
			exit((perror("error"), -1));
		if (tcgetattr(STDIN_FILENO, &term1) != 0)
			exit((perror("error"), -1));
	}
}

void	ft_signals(void)
{
	tcsa();
	signal(SIGINT, ctrl_c);
	signal(SIGQUIT, coix);
	return ;
}
