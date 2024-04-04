/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_get.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: burkaya <burkaya@student.42istanbul.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/25 17:37:15 by burkaya           #+#    #+#             */
/*   Updated: 2024/04/04 23:37:32 by burkaya          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	ft_get_cmd_path(t_token *start_token, t_state *s, t_exec *exec)
{
	char	*env;
	char	**paths;

	env = ft_get_env(s->env, "PATH");
	if (ft_strchr(start_token->value, '/') != NULL)
		return (ft_find_absoulute_path(start_token, exec));
	if (!ft_is_in_cwd(s, exec, start_token->value))
		return (0);
	if (!env)
		return (ft_error(ERR_CMD_NOT_FOUND, start_token->value, 0), \
			ERR_CMD_NOT_FOUND);
	paths = ft_split(env, ':', s);
	return (ft_find_rela_path(paths, start_token, s, exec));
}

static char	**ft_get_args(t_state *s, t_token *tokens, char *cmd_name)
{
	char	**args;
	int		i;
	int		arg_amount;

	i = 0;
	arg_amount = ft_find_arg_amount(tokens);
	args = malloc(sizeof(char *) * (arg_amount + 2));
	ft_add_garbage(s, args);
	args[0] = ft_strdup(cmd_name, s);
	args[arg_amount + 1] = NULL;
	while (tokens)
	{
		if (tokens->type == T_ARG && !ft_is_redirection(tokens->prev))
			args[++i] = ft_strdup(tokens->value, s);
		tokens = tokens->next;
	}
	return (args);
}

int	get_all_cmd(t_exec *exec, t_state *s, t_token *tmp, t_token *tmp1)
{
	int	err;
	int	err1;

	err = 0;
	if (!exec)
		return (1);
	ft_add_garbage(s, exec);
	ft_is_without_cmd(exec);
	exec->is_here_doc = ft_is_here_doc(tmp1);
	exec->count_heredocs = ft_count_heredocs(tmp1);
	err = ft_init_redirections(tmp1, exec, s);
	if (ft_is_builtin(tmp->value))
		exec->type = CMD_BUILTIN;
	if (exec->type == CMD_PATH)
	{
		err1 = ft_get_cmd_path(tmp, s, exec);
		if (err)
			return (err);
		else if (err1 && !(exec->type == CMD_BUILTIN))
			return (err1);
	}
	exec->cmd_args = ft_get_args(s, tmp1, tmp->value);
	if (err)
		return (err);
	return (0);
}
