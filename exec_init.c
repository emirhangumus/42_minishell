/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_init.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: burkaya <burkaya@student.42istanbul.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/07 18:57:02 by burkaya           #+#    #+#             */
/*   Updated: 2024/03/26 14:11:21 by burkaya          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_init_redirections(t_token *tokens, t_exec *exec, t_state *s)
{
	int		i;
	
	i = 0;
	exec->in_type = 0;
	exec->out_type = 0;
	exec->in_fd = 0;
	exec->out_fd = 0;
	exec->in_file = NULL;
	exec->out_file = NULL;
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
			i = ft_open_check_files(exec, i);
		}
		tokens = tokens->next;
	}
	return (i);
}

int	ft_init_execs(t_state *s, t_exec **exec)
{
	t_token		**tmp;
	t_token		**tmp1;
	int			i;
	int			j;
	int			err;

	err = 0;
	j = -1;
	i = -1;
	tmp = s->tokens;
	tmp1 = s->tokens;
	while (tmp[++i])
	{
		while (tmp[i])
		{
			if (tmp[i]->type == T_CMD)
			{
				exec[++j] = malloc(sizeof(t_exec));
				err = get_all_cmd(exec[j], s, tmp[i], tmp1[i]);
			}
			tmp[i] = tmp[i]->next;
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
