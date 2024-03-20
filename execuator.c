/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execuator.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: burkaya <burkaya@student.42istanbul.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/29 14:13:59 by burkaya           #+#    #+#             */
/*   Updated: 2024/03/20 10:35:20 by burkaya          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	exec_one_command(t_state *s, t_exec **exec)
{
	int	fd;
	
	if (exec[0]->type == CMD_BUILTIN)
	{
		fd = dup(1);
		s->status = ft_execute_builtin(s, exec[0]);
		close_redir_fd(exec[0], fd);
		return ;
	}
	s->forks[0] = fork();
	if (s->forks[0] == 0)
	{
		if (exec[0]->in_type || exec[0]->out_type)
			ft_dup_redictions(exec[0]);
		close_redir_fd(exec[0], 1);
		execve(exec[0]->cmd_path, exec[0]->cmd_args, s->env);
	}
	waitpid(s->forks[0], (int *)&s->status, 0);
}

// void	ft_run_without_redirects(t_state *s, t_exec **exec, int cmd_amount, int i)
// {

// }

// void	ft_run_with_redirects(t_state *s, t_exec **exec, int cmd_amount, int i)
// {
	// if (i == 0)
	// {
	// 	ft_dup_redictions(exec[i]);
	// 	close_redir_pipe_fd(exec[i], s->pipes, cmd_amount, i);
	// 	execve(exec[i]->cmd_path, exec[i]->cmd_args, s->env);
	// }
	// else if (i == cmd_amount - 1)
	// {
	// 	ft_dup_redictions(exec[i]);
	// 	close_redir_pipe_fd(exec[i], s->pipes, cmd_amount, i);
	// 	execve(exec[i]->cmd_path, exec[i]->cmd_args, s->env);
	// }
	// else
	// {
	// 	ft_dup_redictions(exec[i]);
	// 	close_redir_pipe_fd(exec[i], s->pipes, cmd_amount, i);
	// 	execve(exec[i]->cmd_path, exec[i]->cmd_args, s->env);
	// }
// }

void	ft_run_pipes(t_state *s, t_exec **exec, int cmd_amount, int i)
{
	if (!exec[i]->in_type && !exec[i]->out_type)
	{
		if (i == 0)
		{
			dup2(s->pipes[i * 2 + 1], 1);
			close_pipes_all(s->pipes, cmd_amount, i);
			if (exec[i]->type == CMD_BUILTIN)
			{
				s->status = ft_execute_builtin(s, exec[i]);
				exit(s->status);
			}
			execve(exec[i]->cmd_path, exec[i]->cmd_args, s->env);
		}
		else if (i == cmd_amount - 1)
		{
			dup2(s->pipes[(i - 1) * 2], 0);
			close_pipes_all(s->pipes, cmd_amount, i);
			if (exec[i]->type == CMD_BUILTIN)
			{
				s->status = ft_execute_builtin(s, exec[i]);
				exit(s->status);
			}
			execve(exec[i]->cmd_path, exec[i]->cmd_args, s->env);
		}
		else
		{
			dup2(s->pipes[(i - 1) * 2], 0);
			dup2(s->pipes[i * 2 + 1], 1);
			close_pipes_all(s->pipes, cmd_amount, i);
			if (exec[i]->type == CMD_BUILTIN)
			{
				s->status = ft_execute_builtin(s, exec[i]);
				exit(s->status);
			}
			execve(exec[i]->cmd_path, exec[i]->cmd_args, s->env);
		}
	}
	else
	{
		if (i == 0)
		{
			ft_dup_redictions(exec[i]);
			close_redir_pipe_fd(exec[i], s->pipes, cmd_amount, i);
			execve(exec[i]->cmd_path, exec[i]->cmd_args, s->env);
		}
		else if (i == cmd_amount - 1)
		{
			ft_dup_redictions(exec[i]);
			close_redir_pipe_fd(exec[i], s->pipes, cmd_amount, i);
			execve(exec[i]->cmd_path, exec[i]->cmd_args, s->env);
		}
		else
		{
			ft_dup_redictions(exec[i]);
			close_redir_pipe_fd(exec[i], s->pipes, cmd_amount, i);
			execve(exec[i]->cmd_path, exec[i]->cmd_args, s->env);
		}
	}
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
	// ft_print_execs(exec);
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
