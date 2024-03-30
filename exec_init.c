/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_init.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: burkaya <burkaya@student.42istanbul.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/07 18:57:02 by burkaya           #+#    #+#             */
/*   Updated: 2024/03/30 09:04:47 by burkaya          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*ft_is_here_doc(t_token *token)
{
	while (token)
	{
		if (token->type == T_LAPPEND)
			return (token->next->value);
		token = token->next;
	}
	return (NULL);
}

int	ft_init_redirections(t_token *tokens, t_exec *exec, t_state *s)
{
	int		i;

	i = 0;
	exec->is_here_doc = ft_is_here_doc(tokens);
	while (tokens)
	{
		if (ft_is_redirection(tokens) && tokens->next)
		{
			if (tokens->type == T_LREDIR || tokens->type == T_LAPPEND)
			{
				exec->in_type = tokens->type;
				if (tokens->type == T_LREDIR)
					exec->in_file = ft_strdup(tokens->next->value, s);
			}
			else if (tokens->type == T_RREDIR || tokens->type == T_RAPPEND)
			{
				exec->out_type = tokens->type;
				exec->out_file = ft_strdup(tokens->next->value, s);
			}
			i = ft_open_check_files(exec, i);
		}
		tokens = tokens->next;
	}
	return (i);
}

void	ft_fill_execs(t_exec *exec)
{
	exec->is_here_doc = 0;
	exec->should_run = 0;
	exec->in_type = 0;
	exec->out_type = 0;
	exec->in_fd = 0;
	exec->out_fd = 0;
	exec->in_file = NULL;
	exec->out_file = NULL;
	exec->type = CMD_PATH;
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
			if (next->type == T_CMD)
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
