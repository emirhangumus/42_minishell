/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_redirs.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: burkaya <burkaya@student.42istanbul.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/20 01:02:10 by burkaya           #+#    #+#             */
/*   Updated: 2024/03/30 14:12:26 by burkaya          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_heredoc(t_exec *exec)
{
	int		pipe_fd[2];
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
