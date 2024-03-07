/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exec_init.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: burkaya <burkaya@student.42istanbul.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/07 18:57:02 by burkaya           #+#    #+#             */
/*   Updated: 2024/03/07 18:58:53 by burkaya          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**ft_get_args(t_state *s, t_token *tokens)
{
	char	**args;
	int		i;
	int		arg_amount;

	i = 1;
	arg_amount = ft_find_arg_amount(tokens);
	args = malloc(sizeof(char *) * (arg_amount + 2));
	ft_add_garbage(s, args);
	args[0] = tokens->value;
	tokens = tokens->next;
	if (args == NULL)
		return (NULL);
	args[arg_amount + 1] = NULL;
	while (i <= arg_amount)
	{
		args[i] = ft_strdup(tokens->value, s);
		tokens = tokens->next;
		i++;
	}
	return (args);
}

int	ft_init_execs(t_state *s, t_exec **exec)
{
	int		j;
	t_token	*tmp;

	j = -1;
	tmp = s->tokens;
	while (tmp)
	{
		if (tmp->type == T_CMD)
		{
			exec[++j] = malloc(sizeof(t_exec));
			ft_add_garbage(s, exec[j]);
			if (exec[j] == NULL)
				return (1);
			if (ft_is_builtin(tmp->value))
				exec[j]->type = CMD_BUILTIN;
			else
				exec[j]->type = CMD_PATH;
			exec[j]->cmd_path = ft_get_cmd_path(tmp, s);
			if (exec[j]->cmd_path == NULL && !(exec[j]->type == CMD_BUILTIN))
				return (ERR_CMD_NOT_FOUND);
			exec[j]->cmd_args = ft_get_args(s, tmp);
		}
		tmp = tmp->next;
	}
	return (0);
}

void	ft_init_pipes(t_state *s, int cmd_amount)
{
	int	i;

	i = 0;
	s->pipes = malloc(sizeof(int) * (cmd_amount * 2));
	s->forks = malloc(sizeof(int) * (cmd_amount));
	while (i < cmd_amount)
	{
		if (pipe(s->pipes + i * 2) < 0)
		{
			s->status = ERR_PIPE_INIT;
			return ;
		}
		i++;
	}
}
