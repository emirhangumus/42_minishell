/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_fclose.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: burkaya <burkaya@student.42istanbul.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/30 13:57:56 by burkaya           #+#    #+#             */
/*   Updated: 2024/03/30 14:37:36 by burkaya          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	ft_close_for_fisrst_pipe(t_exec *exec, int *pipes, int i)
{
	if ((exec->in_file || exec->is_here_doc) && exec->out_fd)
	{
		close(exec->in_fd);
		close(exec->out_fd);
	}
	else if (exec->in_file || exec->is_here_doc)
	{
		close(exec->in_fd);
		close(pipes[i * 2 + 1]);
	}
	else if (exec->out_fd)
		close(exec->out_fd);
}

static void	ft_close_for_last_pipe(t_exec *exec, int *pipes, int i)
{
	if ((exec->in_file || exec->is_here_doc) && exec->out_fd)
	{
		close(exec->in_fd);
		close(exec->out_fd);
	}
	else if (exec->in_file || exec->is_here_doc)
		close(exec->in_fd);
	else if (exec->out_fd)
	{
		close(exec->out_fd);
		close(pipes[(i - 1) * 2]);
	}
}

static void	ft_close_for_middle_pipe(t_exec *exec, int *pipes, int i)
{
	if ((exec->in_file || exec->is_here_doc) && exec->out_fd)
	{
		close(exec->in_fd);
		close(exec->out_fd);
	}
	else if (exec->in_file || exec->is_here_doc)
	{
		close(exec->in_fd);
		close(pipes[i * 2 + 1]);
	}
	else if (exec->out_fd)
	{
		close(exec->out_fd);
		close(pipes[(i - 1) * 2]);
	}
}

int	close_redir_pipe_fd(t_state *s, t_exec *exec, int i)
{
	if (i == 0)
		ft_close_for_fisrst_pipe(exec, s->pipes, i);
	else if (i == s->cmd_amount - 1)
		ft_close_for_last_pipe(exec, s->pipes, i);
	else
		ft_close_for_middle_pipe(exec, s->pipes, i);
	return (0);
}

int	close_redir_fd(t_exec *exec, int fd1, int fd2)
{
	if (exec->in_fd)
		close(exec->in_fd);
	if (exec->out_fd)
		close(exec->out_fd);
	if (exec->type == CMD_BUILTIN)
	{
		dup2(fd1, 1);
		dup2(fd2, 0);
	}
	if (exec->type == CMD_BUILTIN)
	{
		close(fd1);
		close(fd2);
	}
	return (0);
}
