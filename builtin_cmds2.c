/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cmds2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: burkaya <burkaya@student.42istanbul.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/01 01:29:08 by egumus            #+#    #+#             */
/*   Updated: 2024/04/04 23:29:50 by burkaya          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_echo_is_valid(char *arg)
{
	int	i;

	i = 0;
	if (arg[i] == '-')
		i++;
	if (arg[i] == 'n')
	{
		i++;
		while (arg[i] == 'n')
			i++;
		if (arg[i] == '\0')
			return (1);
	}
	return (0);
}

int	ft_echo(t_exec *exec)
{
	int	i;
	int	n_flag;

	i = 1;
	n_flag = 0;
	while (exec->cmd_args[i] && ft_echo_is_valid(exec->cmd_args[i]))
	{
		n_flag = 1;
		i++;
	}
	while (exec->cmd_args[i])
	{
		write(STDOUT_FILENO, exec->cmd_args[i], ft_strlen(exec->cmd_args[i]));
		if (exec->cmd_args[i + 1])
			write(STDOUT_FILENO, " ", 1);
		i++;
	}
	if (!n_flag)
		write(STDOUT_FILENO, "\n", 1);
	return (0);
}

int	ft_env(t_state *s)
{
	int	i;

	i = 0;
	while (s->env[i])
	{
		printf("%s\n", s->env[i]);
		i++;
	}
	return (0);
}

int	ft_pwd(t_state *s)
{
	printf("%s\n", s->cwd);
	return (0);
}
