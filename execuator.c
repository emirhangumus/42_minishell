/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execuator.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: burkaya <burkaya@student.42istanbul.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/29 14:13:59 by burkaya           #+#    #+#             */
/*   Updated: 2024/04/01 06:12:33 by burkaya          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_check_cmd_types(t_exec **exec, int cmd_amount)
{
	int	i;
	int	control;

	control = 0;
	i = 0;
	while (exec[i])
	{
		if (!exec[i]->cmd_path && exec[i]->type != CMD_BUILTIN)
			control++;
		i++;
	}
	if (control == cmd_amount)
		return (1);
	return (0);
}

void	ft_check_redir_exist(t_token *tokens)
{
	t_token	*tmp;

	tmp = tokens;
	while (tmp)
	{
		if (tmp->type == T_RREDIR || tmp->type == T_RAPPEND)
			open(tmp->next->value, O_CREAT | O_WRONLY | O_TRUNC, 0644);
		tmp = tmp->next;
	}
}

int	ft_check_cmd_exist(t_token **token)
{
	t_token	*tmp1;
	int		i;
	int		j;

	j = 0;
	i = 0;
	while (token[j])
	{
		tmp1 = token[j];
		while (tmp1)
		{
			if (tmp1->type == T_CMD)
				i++;
			tmp1 = tmp1->next;
		}
		if (i == 0)
			ft_check_redir_exist(token[j]);
		j++;
		i = 0;
	}
	return (0);
}

int	ft_execuator(t_state *s)
{
	t_exec	**exec;

	// ft_check_cmd_exist(s->tokens);
	ft_print_tokens(s->tokens);
	s->cmd_amount = ft_amount_cmd(s->tokens);
	if (s->cmd_amount == 0)
		return (0);
	exec = malloc(sizeof(t_exec *) * (s->cmd_amount + 1));
	ft_add_garbage(s, exec);
	exec[s->cmd_amount] = NULL;
	s->status = ft_init_execs(s, exec);
	// ft_print_execs(exec);
	if ((s->status && s->cmd_amount == 1) \
		|| (exec[s->cmd_amount - 1]->should_run \
		&& exec[s->cmd_amount - 1]->type == CMD_BUILTIN))
		return (s->status);
	if (ft_check_cmd_types(exec, s->cmd_amount))
	{
		s->status = ERR_CMD_NOT_FOUND;
		return (s->status);
	}
	ft_init_pipes(s);
	ft_exec_hub(s, exec);
	return (0);
}
