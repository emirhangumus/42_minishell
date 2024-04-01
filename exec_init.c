/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_init.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: burkaya <burkaya@student.42istanbul.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/07 18:57:02 by burkaya           #+#    #+#             */
/*   Updated: 2024/04/01 06:15:44 by burkaya          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*ft_is_here_doc(t_token *token)
{
	while (token)
	{
		if (token->type == T_LAPPEND)
			return (token->next->value);
		token = token->next;
	}
	return (NULL);
}

int	ft_count_heredocs(t_token *token)
{
	int	count;

	count = 0;
	while (token)
	{
		if (token->type == T_LAPPEND)
			count++;
		token = token->next;
	}
	return (count);
}

int	ft_init_redirections(t_token *tokens, t_exec *exec, t_state *s)
{
	int		i;

	i = 0;
	exec->is_here_doc = ft_is_here_doc(tokens);
	exec->count_heredocs = ft_count_heredocs(tokens);
	exec->heredocs = malloc(sizeof(char *) * (exec->count_heredocs + 1));
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

static void	ft_fill_execs(t_exec *exec)
{
	exec->is_here_doc = 0;
	exec->should_run = 0;
	exec->in_type = 0;
	exec->out_type = 0;
	exec->in_fd = 0;
	exec->out_fd = 0;
	exec->count_heredocs = 0;
	exec->here_doc_idx = 0;
	exec->in_file = NULL;
	exec->out_file = NULL;
	exec->type = CMD_PATH;
	exec->heredocs = NULL;
}

int	ft_is_without_cmd_redirect(t_token *tokens, int cmd_amount, int j)
{
	int	i;
	t_token	*tmp;
	i = 0;
	tmp = tokens;

	if (j == cmd_amount - 1)
	{
		
		return (0);
	}
	while (tmp)
	{
		if (tmp->type == T_CMD)
			i++;
		if (i != 0)
			return (0);
		tmp = tmp->next;
	}
	if (i == 0)
	{
		tmp = tokens;
		while (tmp)
		{
			if (ft_is_redirection(tmp))
				return (1);
			tmp = tmp->next;
		}
	}
	return (0);
}

int	ft_init_execs(t_state *s, t_exec **exec)
{
	t_token		**tmp;
	t_token		*next;
	int			i;
	int			j;
	int			err;

	err = 0;
	j = -1;
	i = -1;
	tmp = s->tokens;
	while (tmp[++i])
	{
		next = tmp[i];
		while (next)
		{
			if (next->type == T_CMD || ft_is_without_cmd_redirect(next, s->cmd_amount, j))
			{
				exec[++j] = malloc(sizeof(t_exec));
				ft_fill_execs(exec[j]);
				err = get_all_cmd(exec[j], s, next, tmp[i]);
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
