/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_redirs.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: burkaya <burkaya@student.42istanbul.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/20 01:02:10 by burkaya           #+#    #+#             */
/*   Updated: 2024/03/20 09:55:33 by burkaya          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	close_redir_pipe_fd(t_exec *exec, int *pipes, int cmd_amount, int i)
{
	if (i == 0)
	{	
		if (exec->in_file && exec->out_fd)
		{
			mother_close_pipes_all(pipes, cmd_amount);
			return ;
		}
		else if (exec->in_file)
		{
			close_pipes_all(pipes, cmd_amount, i);
			return ;
		}
		else if (exec->out_fd)
		{
			close_pipes_all(pipes, cmd_amount, i);
			close(pipes[i * 2 + 1]);
			return ;
		}
	}
	else if (i == cmd_amount - 1)
	{
		if (exec->in_file && exec->out_fd)
		{
			close_pipes_all(pipes, cmd_amount, i);
			close(pipes[(i - 1) * 2]);
			return ;
		}
		else if (exec->in_file)
		{
			close_pipes_all(pipes, cmd_amount, i);
			close(pipes[(i - 1) * 2]);
			return ;
		}
		else if (exec->out_fd)
		{
			close_pipes_all(pipes, cmd_amount, i);
			close(pipes[(i - 1) * 2]);
			return ;
		}
	}
	else
	{
		if (exec->in_file && exec->out_fd)
		{
			close_pipes_all(pipes, cmd_amount, i);
			return ;
		}
		else if (exec->in_file)
		{
			close_pipes_all(pipes, cmd_amount, i);
			close(pipes[(i - 1) * 2]);
			return ;
		}
		else if (exec->out_fd)
		{
			close_pipes_all(pipes, cmd_amount, i);
			close(pipes[i * 2 + 1]);
			return ;
		}
	
	}
}

int	close_redir_fd(t_exec *exec, int fd)
{
	if (exec->in_fd)
		close(exec->in_fd);
	if (exec->out_fd)
		close(exec->out_fd);
	dup2(fd, 1);
	return (0);
}

void	ft_dup_redictions(t_exec *exec)
{
	if (exec->in_file)
	{
		if (exec->in_type == T_LREDIR)
			exec->in_fd = open(exec->in_file, O_RDONLY);
		else if (exec->in_type == T_LAPPEND)
			exec->in_fd = open(exec->in_file, O_RDONLY);
		dup2(exec->in_fd, 0);
	}
	if (exec->out_file)
	{
		if (exec->out_type == T_RREDIR)
			exec->out_fd = open(exec->out_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		else if (exec->out_type == T_RAPPEND)
			exec->out_fd = open(exec->out_file, O_WRONLY | O_CREAT | O_APPEND, 0644);
		dup2(exec->out_fd, 1);
	}
}

int	ft_is_redirection(t_token *token)
{
	if (token->type == T_LREDIR)
		return (T_LREDIR);
	else if (token->type == T_RREDIR)
		return (T_RREDIR);
	else if (token->type == T_LAPPEND)
		return (T_LAPPEND);
	else if (token->type == T_RAPPEND)
		return (T_RAPPEND);
	return (0);
}