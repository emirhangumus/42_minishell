/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execuator.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: burkaya <burkaya@student.42istanbul.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/29 14:13:59 by burkaya           #+#    #+#             */
/*   Updated: 2024/03/07 18:57:26 by burkaya          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_run_pipes(t_state *s, t_exec **exec, int cmd_amount, int i)
{
	if (i == 0)
	{
		dup2(s->pipes[i * 2 + 1], 1);
		close_pipes_all(s->pipes, cmd_amount, i);
		execve(exec[i]->cmd_path, exec[i]->cmd_args, s->env);
	}
	else if (i == cmd_amount - 1)
	{
		dup2(s->pipes[(i - 1) * 2], 0);
		close_pipes_all(s->pipes, cmd_amount, i);
		execve(exec[i]->cmd_path, exec[i]->cmd_args, s->env);
	}
	else
	{
		dup2(s->pipes[(i - 1) * 2], 0);
		dup2(s->pipes[i * 2 + 1], 1);
		close_pipes_all(s->pipes, cmd_amount, i);
		execve(exec[i]->cmd_path, exec[i]->cmd_args, s->env);
	}
}

void	ft_lets_go(t_state *s, t_exec **exec, int cmd_amount)
{
	int	i;

	i = -1;
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
		waitpid(s->forks[i], &s->status, 0);
		i--;
	}
}

int	ft_execuator(t_state *s)
{
	t_exec	**exec;
	int		cmd_amount;
	int		err_no;
	int		i;

	i = 0;
	cmd_amount = ft_amount_cmd(s->tokens);
	exec = malloc(sizeof(t_exec *) * (cmd_amount + 1));
	ft_add_garbage(s, exec);
	exec[cmd_amount] = NULL;
	err_no = ft_init_execs(s, exec);
	if (err_no)
		return (err_no);
	ft_init_pipes(s, cmd_amount);
	ft_lets_go(s, exec, cmd_amount);
	return (0);
}
