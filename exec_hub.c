/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_hub.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: burkaya <burkaya@student.42istanbul.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/30 13:52:04 by burkaya           #+#    #+#             */
/*   Updated: 2024/04/01 15:07:22 by burkaya          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	ft_run_pipes(t_state *s, t_exec **exec, int i)
{
	if (exec[i]->type == CMD_WITHOUT_CMD)
	{
		mother_close_pipes_all(s);
		exit(0);
	}
	if (!exec[i]->cmd_path && exec[i]->type != CMD_BUILTIN)
	{
		mother_close_pipes_all(s);
		exit(127);
	}
	else if (!exec[i]->in_type && !exec[i]->out_type)
		ft_run_commands(s, exec, i);
	else
		ft_run_redirects(s, exec, i);
}

static void	ft_exec_part(t_state *s, t_exec **exec)
{
	int	i;

	i = -1;
	while (exec[++i])
	{
		if (exec[i]->in_fd == -1 || exec[i]->out_fd == -1)
			continue ;
		s->forks[i] = fork();
		if (s->forks[i] == 0)
			ft_run_pipes(s, exec, i);
	}
}

static void	ft_wait_part(t_state *s)
{
	int	i;

	i = s->cmd_amount - 1;
	while (i >= 0)
	{
		if (i == s->cmd_amount - 1)
		{
			waitpid(s->forks[i], &s->status, 0);
			s->status = WEXITSTATUS(s->status);
		}
		waitpid(s->forks[i], NULL, 0);
		i--;
	}
}

static void	ft_init_here_docs(t_state *s, t_exec **exec)
{
	int	j;

	j = -1;
	while (++j < s->cmd_amount)
	{
		if (exec[j]->is_here_doc)
			ft_heredoc(exec[j]);
	}
}

void	ft_exec_hub(t_state *s, t_exec **exec)
{
	ft_init_here_docs(s, exec);
	g_qsignal = 1;
	if (s->cmd_amount > 1)
	{
		ft_exec_part(s, exec);
		mother_close_pipes_all(s);
		ft_wait_part(s);
	}
	else
		s->status = exec_one_command(s, exec);
	g_qsignal = 0;
}
