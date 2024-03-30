/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_dupered.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: burkaya <burkaya@student.42istanbul.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/30 14:11:14 by burkaya           #+#    #+#             */
/*   Updated: 2024/03/30 14:30:48 by burkaya          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	ft_init_dupes_first(t_exec *exec, t_state *s, int i)
{
	if ((exec->in_file || exec->is_here_doc) && exec->out_file)
		mother_close_pipes_all(s);
	else if (exec->in_file || exec->is_here_doc)
	{
		dup2(s->pipes[i * 2 + 1], 1);
		close_pipes_all(s->pipes, s->cmd_amount, i);
	}
	else if (exec->out_file)
		mother_close_pipes_all(s);
}

static void	ft_init_dupes_last(t_exec *exec, t_state *s, int i)
{
	if ((exec->in_file || exec->is_here_doc) && exec->out_file)
		mother_close_pipes_all(s);
	else if (exec->in_file || exec->is_here_doc)
		mother_close_pipes_all(s);
	else if (exec->out_file)
	{
		dup2(s->pipes[(i - 1) * 2], 0);
		close_pipes_all(s->pipes, s->cmd_amount, i);
	}
}

static void	ft_init_dupes_midd(t_exec *exec, t_state *s, int i)
{
	if ((exec->in_file || exec->is_here_doc) && exec->out_file)
		mother_close_pipes_all(s);
	else if (exec->in_file || exec->is_here_doc)
	{
		dup2(s->pipes[i * 2 + 1], 1);
		close_pipes_all(s->pipes, s->cmd_amount, i);
		close(s->pipes[(i - 1) * 2]);
	}
	else if (exec->out_file)
	{
		dup2(s->pipes[(i - 1) * 2], 0);
		close_pipes_all(s->pipes, s->cmd_amount, i);
		close(s->pipes[i * 2 + 1]);
	}
}

void	ft_init_dupes(t_state *s, t_exec *exec, int i)
{
	if (i == 0)
		ft_init_dupes_first(exec, s, i);
	else if (i == s->cmd_amount - 1)
		ft_init_dupes_last(exec, s, i);
	else
		ft_init_dupes_midd(exec, s, i);
}
