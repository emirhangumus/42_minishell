/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_run.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: burkaya <burkaya@student.42istanbul.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/20 11:04:11 by burkaya           #+#    #+#             */
/*   Updated: 2024/03/30 09:30:03 by burkaya          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
//	if (exec[0]->in_fd == -1 || exec[0]->out_fd == -1) err

int	exec_one_command(t_state *s, t_exec **exec)
{
	int	fd1;
	int	fd2;
	if (exec[0]->should_run && !exec[0]->is_here_doc)
		return (1);
	if ((exec[0]->in_fd == -1 || exec[0]->out_fd == -1) && !exec[0]->is_here_doc)
		return (1);
	if (exec[0]->is_here_doc)
		ft_heredoc(exec[0]);
	if (exec[0]->type == CMD_BUILTIN)
	{
		fd1 = dup(1);
		fd2 = dup(0);
		s->status = ft_execute_builtin(s, exec[0]);
		close_redir_fd(exec[0], fd1, fd2);
		return (s->status);
	}
	s->forks[0] = fork();
	if (s->forks[0] == 0)
	{
		if (exec[0]->in_type || exec[0]->out_type)
			ft_dup_redictions(exec[0], s);
		if (exec[0]->should_run)
			exit(1);
		if (execve(exec[0]->cmd_path, exec[0]->cmd_args, s->env) == -1)
			exit(1);
	}
	waitpid(s->forks[0], (int *)&s->status, 0);
	if (WIFEXITED(s->status))
		s->status = WEXITSTATUS(s->status);
	return (s->status);
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
	if (exec[i]->should_run)
		ft_run_commands(s, exec, cmd_amount, i);
	if (i == 0)
	{
		ft_dup_redictions(exec[i], s);
		ft_init_dupes(exec[i], s->pipes, cmd_amount, i);
		if (exec[i]->type == CMD_BUILTIN)
		{
			s->status = ft_execute_builtin(s, exec[i]);
			exit(s->status);
		}
		close_redir_pipe_fd(exec[i], s->pipes, cmd_amount, i);
		execve(exec[i]->cmd_path, exec[i]->cmd_args, s->env);
	}
	else if (i == cmd_amount - 1)
	{
		ft_dup_redictions(exec[i], s);
		ft_init_dupes(exec[i], s->pipes, cmd_amount, i);
		if (exec[i]->type == CMD_BUILTIN)
		{
			s->status = ft_execute_builtin(s, exec[i]);
			exit(s->status);
		}
		close_redir_pipe_fd(exec[i], s->pipes, cmd_amount, i);
		execve(exec[i]->cmd_path, exec[i]->cmd_args, s->env);
	}
	else
	{
		ft_dup_redictions(exec[i], s);
		ft_init_dupes(exec[i], s->pipes, cmd_amount, i);
		if (exec[i]->type == CMD_BUILTIN)
		{
			s->status = ft_execute_builtin(s, exec[i]);
			exit(s->status);
		}
		close_redir_pipe_fd(exec[i], s->pipes, cmd_amount, i);
		execve(exec[i]->cmd_path, exec[i]->cmd_args, s->env);
	}
}
