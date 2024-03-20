/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execuator.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: burkaya <burkaya@student.42istanbul.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/29 14:13:59 by burkaya           #+#    #+#             */
/*   Updated: 2024/03/20 14:02:46 by burkaya          ###   ########.fr       */
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

void	ft_lets_go(t_state *s, t_exec **exec, int cmd_amount)
{
	int	i;

	i = -1;
	if (cmd_amount > 1)
	{
		while (exec[++i])
		{
			s->forks[i] = fork();
			if (s->forks[i] == 0)
				ft_run_pipes(s, exec, cmd_amount, i);
		}
		mother_close_pipes_all(s->pipes, cmd_amount);
		i = cmd_amount - 1;
		while (i >= 0)
		{
			waitpid(s->forks[i], (int *)&s->status, 0);
			i--;
		}
	}
	else
		exec_one_command(s, exec);
}

int	ft_execuator(t_state *s)
{
	t_exec	**exec;
	int		cmd_amount;
	int		err_no;

	cmd_amount = ft_amount_cmd(s->tokens);
	exec = malloc(sizeof(t_exec *) * (cmd_amount + 1));
	ft_add_garbage(s, exec);
	exec[cmd_amount] = NULL;
	err_no = ft_init_execs(s, exec);
	if (err_no)
		return (err_no);
	ft_print_execs(exec);
	ft_init_pipes(s, cmd_amount);
	ft_lets_go(s, exec, cmd_amount);
	return (0);
}

/*
T_APPEND = <<
T_LREDIR = <
T_RAPPEND = >>
T_RREDIR = >
*/
