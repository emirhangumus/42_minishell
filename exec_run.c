/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_run.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: burkaya <burkaya@student.42istanbul.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/20 11:04:11 by burkaya           #+#    #+#             */
/*   Updated: 2024/03/25 17:55:57 by burkaya          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	exec_one_command(t_state *s, t_exec **exec)
{
	int	fd;

	if (exec[0]->type == CMD_BUILTIN)
	{
		fd = dup(1);
		s->status = ft_execute_builtin(s, exec[0]);
		close_redir_fd(exec[0], fd);
		return (0);
	}
	if (exec[0]->in_fd == -1 || exec[0]->out_fd == -1)
		return (1);
	s->forks[0] = fork();
	if (s->forks[0] == 0)
	{
		if (exec[0]->in_type || exec[0]->out_type)
			ft_dup_redictions(exec[0], s);
		execve(exec[0]->cmd_path, exec[0]->cmd_args, s->env);
	}
	waitpid(s->forks[0], (int *)&s->status, 0);
	return (0);
}

void	ft_run_commands(t_state *s, t_exec **exec, int cmd_amount, int i)
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

void	ft_run_redirects(t_state *s, t_exec **exec, int cmd_amount, int i)
{
	if (i == 0)
	{
		ft_init_dupes(exec[i], s->pipes, cmd_amount, i);
		if (exec[i]->type == CMD_BUILTIN)
		{
			s->status = ft_execute_builtin(s, exec[i]);
			exit(s->status);
		}
		ft_dup_redictions(exec[i], s);
		close_redir_pipe_fd(exec[i], s->pipes, cmd_amount, i);
		execve(exec[i]->cmd_path, exec[i]->cmd_args, s->env);
	}
	else if (i == cmd_amount - 1)
	{
		ft_init_dupes(exec[i], s->pipes, cmd_amount, i);
		if (exec[i]->type == CMD_BUILTIN)
		{
			s->status = ft_execute_builtin(s, exec[i]);
			exit(s->status);
		}
		ft_dup_redictions(exec[i], s);
		close_redir_pipe_fd(exec[i], s->pipes, cmd_amount, i);
		execve(exec[i]->cmd_path, exec[i]->cmd_args, s->env);
	}
	else
	{
		ft_init_dupes(exec[i], s->pipes, cmd_amount, i);
		if (exec[i]->type == CMD_BUILTIN)
		{
			s->status = ft_execute_builtin(s, exec[i]);
			exit(s->status);
		}
		ft_dup_redictions(exec[i], s);
		close_redir_pipe_fd(exec[i], s->pipes, cmd_amount, i);
		execve(exec[i]->cmd_path, exec[i]->cmd_args, s->env);
	}
}
