/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_run.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: egumus <egumus@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/20 11:04:11 by burkaya           #+#    #+#             */
/*   Updated: 2024/04/02 21:46:04 by egumus           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	exec_one_command_part(t_state *s, t_exec **exec)
{
	s->forks[0] = fork();
	if (s->forks[0] == 0)
	{
		if (exec[0]->in_type || exec[0]->out_type)
			ft_dup_redictions(exec[0], s);
		if (exec[0]->should_run && exec[0]->is_here_doc)
			ft_error(ERR_NO_FILE_OR_DIR, exec[0]->in_file, 1);
		if (!exec[0]->cmd_path)
			exit(0);
		else if (execve(exec[0]->cmd_path, exec[0]->cmd_args, s->env) == -1)
			exit(1);
	}
}

int	exec_one_command(t_state *s, t_exec **exec)
{
	int	fd1;
	int	fd2;

	if (exec[0]->should_run && !exec[0]->is_here_doc)
		return (1);
	if (exec[0]->type == CMD_BUILTIN)
	{
		fd1 = dup(1);
		fd2 = dup(0);
		s->status = ft_execute_builtin(s, exec[0]);
		return (close_redir_fd(exec[0], fd1, fd2), s->status);
	}
	exec_one_command_part(s, exec);
	waitpid(s->forks[0], (int *)&s->status, 0);
	s->status = WEXITSTATUS(s->status);
	return (s->status);
}

static void	ft_run(t_state *s, t_exec **exec, int cmd_amount, int i)
{
	close_pipes_all(s->pipes, cmd_amount, i);
	if (exec[i]->type == CMD_BUILTIN)
	{
		s->status = ft_execute_builtin(s, exec[i]);
		exit(s->status);
	}
	if (execve(exec[i]->cmd_path, exec[i]->cmd_args, s->env) == -1)
		exit(1);
}

void	ft_run_commands(t_state *s, t_exec **exec, int i)
{
	if (i == 0)
	{
		dup2(s->pipes[i * 2 + 1], 1);
		ft_run(s, exec, s->cmd_amount, i);
	}
	else if (i == s->cmd_amount - 1)
	{
		dup2(s->pipes[(i - 1) * 2], 0);
		ft_run(s, exec, s->cmd_amount, i);
	}
	else
	{
		dup2(s->pipes[(i - 1) * 2], 0);
		dup2(s->pipes[i * 2 + 1], 1);
		ft_run(s, exec, s->cmd_amount, i);
	}
}

void	ft_run_redirects(t_state *s, t_exec **exec, int i)
{
	if (exec[i]->should_run && !exec[i]->is_here_doc)
		ft_run_commands(s, exec, i);
	ft_dup_redictions(exec[i], s);
	ft_init_dupes(s, exec[i], i);
	if (exec[i]->type == CMD_BUILTIN)
	{
		s->status = ft_execute_builtin(s, exec[i]);
		exit(s->status);
	}
	close_redir_pipe_fd(s, exec[i], i);
	if (exec[i]->should_run && exec[i]->is_here_doc)
		ft_error(ERR_NO_FILE_OR_DIR, exec[i]->in_file, 1);
	execve(exec[i]->cmd_path, exec[i]->cmd_args, s->env);
}
