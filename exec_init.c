/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_init.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: burkaya <burkaya@student.42istanbul.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/07 18:57:02 by burkaya           #+#    #+#             */
/*   Updated: 2024/04/04 23:34:29 by burkaya          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_is_without_cmd(t_exec *exec)
{
	if (exec->is_without_cmd)
	{
		exec->type = CMD_WITHOUT_CMD;
		exec->cmd_path = "without_cmd";
	}
}

int	ft_init_redirections(t_token *tokens, t_exec *exec, t_state *s)
{
	int		i;

	i = 0;
	exec->heredocs = malloc(sizeof(char *) * (exec->count_heredocs + 1));
	ft_add_garbage(s, exec->heredocs);
	exec->heredocs[exec->count_heredocs] = NULL;
	while (tokens)
	{
		if (ft_is_redirection(tokens) && tokens->next)
		{
			if (tokens->type == T_LREDIR || tokens->type == T_LAPPEND)
			{
				exec->in_type = tokens->type;
				exec->in_file = ft_strdup(tokens->next->value, s);
			}
			else if (tokens->type == T_RREDIR || tokens->type == T_RAPPEND)
			{
				exec->out_type = tokens->type;
				exec->out_file = ft_strdup(tokens->next->value, s);
			}
			i = ft_open_check_files(exec, i, s);
		}
		tokens = tokens->next;
	}
	return (i);
}

static void	ft_fill_execs(t_exec *exec, int j, t_token *next, t_state *s)
{
	exec->is_here_doc = 0;
	exec->should_run = 0;
	exec->in_type = 0;
	exec->out_type = 0;
	exec->in_fd = 0;
	exec->out_fd = 0;
	exec->err_no = 0;
	exec->err_value = NULL;
	exec->count_heredocs = 0;
	exec->here_doc_idx = 0;
	exec->is_without_cmd = isredwocmd(next, s->cmd_amount, j);
	exec->in_file = NULL;
	exec->out_file = NULL;
	exec->type = CMD_PATH;
	exec->heredocs = NULL;
}

int	ft_init_execs(t_state *s, t_exec **exec)
{
	t_token		*next;
	int			i;
	int			j;
	int			err;

	err = 0;
	j = -1;
	i = -1;
	while (s->tokens[++i])
	{
		next = s->tokens[i];
		while (next)
		{
			if (next->type == T_CMD
				|| isredwocmd(s->tokens[i], s->cmd_amount, j + 1))
			{
				exec[++j] = malloc(sizeof(t_exec));
				ft_fill_execs(exec[j], j, s->tokens[i], s);
				err = get_all_cmd(exec[j], s, next, s->tokens[i]);
				break ;
			}
			next = next->next;
		}
	}
	return (err);
}

void	ft_init_pipes(t_state *s)
{
	int	i;

	i = 0;
	s->pipes = malloc(sizeof(int) * (s->cmd_amount * 2));
	s->forks = malloc(sizeof(int) * (s->cmd_amount));
	while (i < s->cmd_amount)
	{
		if (pipe(s->pipes + i * 2) < 0)
		{
			s->status = ERR_PIPE_INIT;
			return ;
		}
		i++;
	}
}
