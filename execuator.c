/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execuator.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: burkaya <burkaya@student.42istanbul.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/29 14:13:59 by burkaya           #+#    #+#             */
/*   Updated: 2024/03/30 00:13:27 by burkaya          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_run_pipes(t_state *s, t_exec **exec, int cmd_amount, int i)
{
	if (!exec[i]->cmd_path && exec[i]->type != CMD_BUILTIN)
	{
		mother_close_pipes_all(s->pipes, cmd_amount);
		exit(127);
	}
	else if (!exec[i]->in_type && !exec[i]->out_type)
		ft_run_commands(s, exec, cmd_amount, i);
	else
		ft_run_redirects(s, exec, cmd_amount, i);
}

void	ft_lets_go(t_state *s, t_exec **exec)
{
	int	i;

	i = -1;
	if (s->cmd_amount > 1)
	{
		while (exec[++i])
		{
			if (exec[i]->in_fd == -1 || exec[i]->out_fd == -1)
				continue ;
			s->forks[i] = fork();
			if (s->forks[i] == 0)
				ft_run_pipes(s, exec, s->cmd_amount, i);
		}
		mother_close_pipes_all(s->pipes, s->cmd_amount);
		i = s->cmd_amount - 1;
		while (i >= 0)
		{
			if (i == s->cmd_amount - 1)
			{
				waitpid(s->forks[i], &s->status, 0);
				if (WIFEXITED(s->status))
					s->status = WEXITSTATUS(s->status);
			}
			waitpid(s->forks[i], NULL, 0);
			i--;
		}
	}
	else
		s->status = exec_one_command(s, exec);
}

int	ft_check_cmd_types(t_exec **exec, int cmd_amount)
{
	int	i;
	int	control;

	control = 0;
	i = 0;
	while (exec[i])
	{
		if (!exec[i]->cmd_path && exec[i]->type != CMD_BUILTIN)
			control++;
		i++;
	}
	if (control == cmd_amount)
		return (1);
	return (0);
}

int	ft_execuator(t_state *s)
{
	t_exec	**exec;

	s->cmd_amount = ft_amount_cmd(s->tokens);
	if (s->cmd_amount == 0)
		return (0);
	exec = malloc(sizeof(t_exec *) * (s->cmd_amount + 1));
	ft_add_garbage(s, exec);
	exec[s->cmd_amount] = NULL;
	s->status = ft_init_execs(s, exec);
	// ft_print_execs(exec);
	if ((s->status && s->cmd_amount == 1) \
		|| (exec[s->cmd_amount - 1]->should_run \
		&& exec[s->cmd_amount - 1]->type == CMD_BUILTIN))
		return (s->status);
	if (ft_check_cmd_types(exec, s->cmd_amount))
	{
		s->status = ERR_CMD_NOT_FOUND;
		return (s->status);
	}
	ft_init_pipes(s);
	ft_lets_go(s, exec);
	return (0);
}
