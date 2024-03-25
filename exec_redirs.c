/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_redirs.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: burkaya <burkaya@student.42istanbul.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/20 01:02:10 by burkaya           #+#    #+#             */
/*   Updated: 2024/03/25 16:30:02 by burkaya          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_init_dupes(t_exec *exec, int *pipes, int cmd_amount, int i)
{
	if (i == 0)
	{
		if (exec->in_file && exec->out_file)
			mother_close_pipes_all(pipes, cmd_amount);
		else if (exec->in_file)
		{
			dup2(pipes[i * 2 + 1], 1);
			close_pipes_all(pipes, cmd_amount, i);
		}
		else if (exec->out_fd)
		{
			close_pipes_all(pipes, cmd_amount, i);
		}
	}
	else if (i == cmd_amount - 1)
	{
		if (exec->in_file && exec->out_file)
			mother_close_pipes_all(pipes, cmd_amount);
		else if (exec->in_file)
			mother_close_pipes_all(pipes, cmd_amount);
		else if (exec->out_file)
		{
			dup2(pipes[(i - 1) * 2], 0);
			close_pipes_all(pipes, cmd_amount, i);
		}
	}
	else
	{
		if (exec->in_file && exec->out_file)
			mother_close_pipes_all(pipes, cmd_amount);
		else if (exec->in_file)
		{
			dup2(pipes[i * 2 + 1], 1);
			close_pipes_all(pipes, cmd_amount, i);
		}
		else if (exec->out_file)
		{
			dup2(pipes[(i - 1) * 2], 0);
			close_pipes_all(pipes, cmd_amount, i);
		}
	}
}

int	close_redir_pipe_fd(t_exec *exec, int *pipes, int cmd_amount, int i)
{
	if (i == 0)
	{	
		if (exec->in_file && exec->out_fd)
			return (mother_close_pipes_all(pipes, cmd_amount), 1);
		else if (exec->in_file)
			return (close_pipes_all(pipes, cmd_amount, i), 1);
		else if (exec->out_fd)
			return (close_pipes_all(pipes, cmd_amount, i), close(pipes[i * 2 + 1]), 1);
	}
	else if (i == cmd_amount - 1)
	{
		if (exec->in_file && exec->out_fd)
			return (close_pipes_all(pipes, cmd_amount, i), close(pipes[(i - 1) * 2]), 1);
		else if (exec->in_file)
			return (close_pipes_all(pipes, cmd_amount, i), close(pipes[(i - 1) * 2]), 1);
		else if (exec->out_fd)
			return (close_pipes_all(pipes, cmd_amount, i), close(pipes[(i - 1) * 2]), 1);
	}
	else
	{
		if (exec->in_file && exec->out_fd)
			return (close_pipes_all(pipes, cmd_amount, i), 1);
		else if (exec->in_file)
			return (close_pipes_all(pipes, cmd_amount, i), close(pipes[(i - 1) * 2]), 1);
		else if (exec->out_fd)
			return (close_pipes_all(pipes, cmd_amount, i), close(pipes[i * 2 + 1]), 1);
	}
	return (0);
}

int	close_redir_fd(t_exec *exec, int fd)
{
	if (exec->in_fd)
		close(exec->in_fd);
	if (exec->out_fd)
		close(exec->out_fd);
	if (exec->type == CMD_BUILTIN)
		dup2(fd, 1);
	if (exec->type == CMD_BUILTIN)
		close(fd);
	return (0);
}
// 
int	ft_open_check_files(t_exec *exec)
{
	static int i = 0;
	static int j = 0;
	if (exec->in_file)
	{
		if (exec->in_type == T_LREDIR)
			exec->in_fd = open(exec->in_file, O_RDONLY);
		else if (exec->in_type == T_LAPPEND)
			exec->in_fd = open(exec->in_file, O_RDONLY);
		if (exec->in_fd == -1 && i == 0)
		{
			ft_error(ERR_NO_FILE_OR_DIR, exec->in_file, 0);
			i = 1;
		}
		return (1);
	}
	if (exec->out_file)
	{
		if (exec->out_type == T_RREDIR)
			exec->out_fd = open(exec->out_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		else if (exec->out_type == T_RAPPEND)
			exec->out_fd = open(exec->out_file, O_WRONLY | O_CREAT | O_APPEND, 0644);
		if (exec->out_fd == -1 && j == 0)
		{
			ft_error(ERR_NO_FILE_OR_DIR, exec->out_file, 0);
			j = 1;
		}
		return (1);
	}
	return (0);
}

int	ft_dup_redictions(t_exec *exec, t_state *s)
{
	// if (ft_open_check_files(exec))
	// 	return (1);
	if (s->cmd_amount == 1 && exec->in_file && exec->type == CMD_BUILTIN)
	{
		if (exec->in_file)
			close(exec->in_fd);
		return (0);
	}
	if (exec->in_file)
		dup2(exec->in_fd, 0);
	if (exec->out_file)
		dup2(exec->out_fd, 1);	
	return (0);
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
