/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execuator.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: burkaya <burkaya@student.42istanbul.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/29 14:13:59 by burkaya           #+#    #+#             */
/*   Updated: 2024/03/26 14:01:19 by burkaya          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_run_pipes(t_state *s, t_exec **exec, int cmd_amount, int i)
{

	if (!exec[i]->in_type && !exec[i]->out_type)
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
				waitpid(s->forks[i], (int *)&s->status, 0);
			waitpid(s->forks[i], NULL, 0);
			i--;
		}
	}
	else
		s->status = exec_one_command(s, exec);
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
	if ((s->status && s->cmd_amount == 1) || (exec[s->cmd_amount - 1]->should_run && exec[s->cmd_amount - 1]->type == CMD_BUILTIN))
		return (s->status);
	// ft_print_execs(exec);
	ft_init_pipes(s);
	ft_lets_go(s, exec);
	return (0);
}

/*
T_APPEND = <<
T_LREDIR = <
T_RAPPEND = >>
T_RREDIR = >
*/
