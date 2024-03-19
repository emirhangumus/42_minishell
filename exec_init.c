/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_init.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: burkaya <burkaya@student.42istanbul.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/07 18:57:02 by burkaya           #+#    #+#             */
/*   Updated: 2024/03/19 21:03:14 by burkaya          ###   ########.fr       */
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
	args[0] = "value";
	args[arg_amount + 1] = NULL;
	while (tokens)
	{
		if (tokens->type == T_ARG)
		{
			args[i] = ft_strdup(tokens->value, s);
			i++;
		}
		tokens = tokens->next;
	}
	return (args);
	// args[0] = tokens->value;
	// tokens = tokens->next;
	// if (args == NULL)
	// 	return (NULL);
	// args[arg_amount + 1] = NULL;
	// while (i <= arg_amount)
	// {
	// 	args[i] = ft_strdup(tokens->value, s);
	// 	tokens = tokens->next;
	// 	i++;
	// }
}
// echo asdasdasd dasdasdas

int	tab_len(t_token **tab)
{
	int	i;

	i = 0;
	while (tab[i])
		i++;
	return (i);
}

int	ft_init_execs(t_state *s, t_exec **exec)
{
	t_token		**tmp;
	t_token		**tmp1;
	int			i;
	int			j;

	j = 0;
	i = 0;
	tmp = s->tokens;
	tmp1 = s->tokens;
	while (tmp[i])
	{
		while (tmp[i])
		{
			if (tmp[i]->type == T_CMD)
			{
				exec[j] = malloc(sizeof(t_exec));
				ft_add_garbage(s, exec[j]);
				if (!exec[j])
					return (1);
				if (ft_is_builtin(tmp[i]->value))
					exec[j]->type = CMD_BUILTIN;
				else
					exec[j]->type = CMD_PATH;
				exec[j]->cmd_path = ft_get_cmd_path(tmp[i], s);
				if (exec[j]->cmd_path == NULL && !(exec[0]->type == CMD_BUILTIN))
					return (ERR_CMD_NOT_FOUND);
				exec[j]->cmd_args = ft_get_args(s, tmp1[i]);
				j++;
			}
			tmp[i] = tmp[i]->next;
		}
		i++;
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
