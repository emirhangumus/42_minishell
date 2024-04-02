/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_redirs.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: egumus <egumus@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/20 01:02:10 by burkaya           #+#    #+#             */
/*   Updated: 2024/04/03 01:47:31 by egumus           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_heredoc_loop(t_exec *exec, char *buff, int pipe_fd[2])
{
	int	i;

	i = 0;
	while (buff)
	{
		buff = readline("> ");
		if (buff == NULL || !ft_strcmp(exec->heredocs[i], buff))
		{
			if (buff == NULL || i == exec->count_heredocs - 1)
			{
				free(buff);
				break ;
			}
			i++;
			free(buff);
			continue ;
		}
		if (i == exec->count_heredocs - 1)
		{
			write(pipe_fd[1], buff, ft_strlen(buff));
			write(pipe_fd[1], "\n", 1);
		}
		free(buff);
	}
}

void	ft_heredoc(t_exec *exec)
{
	int		pipe_fd[2];
	char	*buff;

	buff = "init_value";
	if (pipe(pipe_fd) == -1)
		return ;
	ft_heredoc_loop(exec, buff, pipe_fd);
	close(pipe_fd[1]);
	exec->in_fd = pipe_fd[0];
}

int	ft_dup_redictions(t_exec *exec, t_state *s)
{
	if (s->cmd_amount == 1 && exec->in_file && exec->type == CMD_BUILTIN \
		&& exec->in_type != T_LAPPEND)
		return (close(exec->in_fd), 0);
	if (exec->in_file && exec->in_type == T_LREDIR && !exec->is_here_doc)
		dup2(exec->in_fd, 0);
	else if (exec->in_file && !exec->is_here_doc)
		dup2(exec->in_fd, 0);
	if (exec->out_file)
		dup2(exec->out_fd, 1);
	if (exec->in_fd && exec->is_here_doc)
		dup2(exec->in_fd, 0);
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
