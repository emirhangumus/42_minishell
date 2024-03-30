/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_redirs.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: burkaya <burkaya@student.42istanbul.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/20 01:02:10 by burkaya           #+#    #+#             */
/*   Updated: 2024/03/30 09:33:24 by burkaya          ###   ########.fr       */
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
		else if (exec->out_file)
			mother_close_pipes_all(pipes, cmd_amount);
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
			close(pipes[(i - 1) * 2]);
		}
		else if (exec->out_file)
		{
			dup2(pipes[(i - 1) * 2], 0);
			close_pipes_all(pipes, cmd_amount, i);
			close(pipes[i * 2 + 1]);
		}
	}
}

int	close_redir_pipe_fd(t_exec *exec, int *pipes, int cmd_amount, int i)
{
	if (i == 0)
	{	
		if (exec->in_file && exec->out_fd)
			return (close(exec->in_fd), close(exec->out_fd), 1);
		else if (exec->in_file)
			return (close(exec->in_fd), close(pipes[i * 2 + 1]), 1);
		else if (exec->out_fd)
			return (close(exec->out_fd), 1);
	}
	else if (i == cmd_amount - 1)
	{
		if (exec->in_file && exec->out_fd)
			return (close(exec->in_fd), close(exec->out_fd), 1);
		else if (exec->in_file)
			return (close(exec->in_fd), 1);
		else if (exec->out_fd)
			return (close(exec->out_fd), close(pipes[(i - 1) * 2]), 1);
	}
	else
	{
		if (exec->in_file && exec->out_fd)
			return (close(exec->in_fd), close(exec->out_fd), 1);
		else if (exec->in_file)
			return (close(exec->in_fd), close(pipes[i * 2 + 1]), 1);
		else if (exec->out_fd)
			return (close(exec->out_fd), close(pipes[(i - 1) * 2]), 1);
	}
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

int	ft_for_in_file(t_exec *exec)
{
	if (!access(exec->in_file, F_OK) && access(exec->in_file, R_OK) == -1)
	{
		exec->should_run = 1;
		if (!exec->is_here_doc)
			ft_error(ERR_PERMISSION_DENIED, exec->in_file, 0);
		return (126);
	}
	exec->should_run = 1;
	if (exec->is_here_doc)
		return (0);
	ft_error(ERR_NO_FILE_OR_DIR, exec->in_file, 0);
	return (ERR_NO_FILE_OR_DIR);
}

int	ft_for_out_file(t_exec *exec)
{
	if (!access(exec->out_file, F_OK) && access(exec->out_file, W_OK) == -1)
	{
		exec->should_run = 1;
		ft_error(ERR_PERMISSION_DENIED, exec->out_file, 0);
		return (1);
	}
	exec->should_run = 1;
	ft_error(ERR_NO_FILE_OR_DIR, exec->out_file, 0);
	return (ERR_NO_FILE_OR_DIR);
}

int	ft_open_check_files(t_exec *exec, int status)
{
	if (exec->in_file)
	{
		if (exec->in_type == T_LREDIR && exec->should_run == 0)
			exec->in_fd = open(exec->in_file, O_RDONLY);
		else if (exec->in_type == T_LAPPEND)
			return (status);			
		if (exec->in_fd == -1 && exec->should_run == 0)
			return (ft_for_in_file(exec));
	}
	if (exec->out_file && exec->should_run == 0)
	{
		if (exec->out_type == T_RREDIR)
			exec->out_fd = open(exec->out_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		else if (exec->out_type == T_RAPPEND)
			exec->out_fd = open(exec->out_file, O_WRONLY | O_CREAT | O_APPEND, 0644);
		if (exec->out_fd == -1 && exec->should_run == 0)
			return (ft_for_out_file(exec));
	}
	return (status);
}

void	ft_heredoc(t_exec *exec)
{
	int	pipe_fd[2];
	char	*buff;

	if (pipe(pipe_fd) == -1)
		return ;
	buff = readline("> ");
	while (buff && ft_strcmp(exec->is_here_doc, buff))
	{
		write(pipe_fd[1], buff, ft_strlen(buff));
		write(pipe_fd[1], "\n", 1);
		free(buff);
		buff = readline("> ");
	}
	free(buff);
	if (exec->should_run)
		ft_error(ERR_NO_FILE_OR_DIR, exec->in_file, 0);
	close(pipe_fd[1]);
	exec->in_fd = pipe_fd[0];
}

int	ft_dup_redictions(t_exec *exec, t_state *s)
{
	if (s->cmd_amount == 1 && exec->in_file && exec->type == CMD_BUILTIN && exec->in_type != T_LAPPEND)
		return (close(exec->in_fd), 0);
	if (exec->in_file && exec->in_type == T_LREDIR && !exec->is_here_doc)
		dup2(exec->in_fd, 0);
	else if (exec->in_file && exec->is_here_doc)
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
