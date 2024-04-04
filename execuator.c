/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execuator.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: burkaya <burkaya@student.42istanbul.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/29 14:13:59 by burkaya           #+#    #+#             */
/*   Updated: 2024/04/04 23:36:04 by burkaya          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_execuator(t_state *s)
{
	t_exec	**exec;

	s->cmd_amount = ft_amount_cmd(s->tokens);
	if (s->cmd_amount == 0)
		return (0);
	exec = malloc(sizeof(t_exec *) * (s->cmd_amount + 1));
	ft_add_garbage(s, exec);
	exec[s->cmd_amount] = NULL;
	s->status = ft_init_execs(s, exec);
	if ((s->status && s->cmd_amount == 1 && !(exec[0]->is_here_doc))
		|| ((exec[s->cmd_amount - 1]->should_run
				&& exec[s->cmd_amount - 1]->type == CMD_BUILTIN)
			&& !(exec[s->cmd_amount - 1]->is_here_doc)))
	{
		if (exec[0]->err_no != 0)
			ft_error(exec[0]->err_no, exec[0]->err_value, 0);
		return (s->status);
	}
	ft_init_pipes(s);
	ft_exec_hub(s, exec);
	return (0);
}
