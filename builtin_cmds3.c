/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cmds1.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: egumus <egumus@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/09 17:00:46 by burkaya           #+#    #+#             */
/*   Updated: 2024/04/01 01:28:47 by egumus           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_export_is_valid(t_exec *exec)
{
	int	i;
	int	j;

	i = 1;
	while (exec->cmd_args[i])
	{
		j = 0;
		while (exec->cmd_args[i][j] && exec->cmd_args[i][j] != '=')
		{
			if (exec->cmd_args[i][j] == '-')
				return (ft_error(ERR_NOT_VALID_IDENTIFIER, \
					exec->cmd_args[i], 0), 1);
			if (!ft_isalnum(exec->cmd_args[i][j]))
				return (ft_error(ERR_NOT_VALID_IDENTIFIER, \
					exec->cmd_args[i], 0), 1);
			j++;
		}
		if (exec->cmd_args[i][j] && !exec->cmd_args[i][j + 1])
			return (ft_error(ERR_NOT_VALID_IDENTIFIER, \
				exec->cmd_args[i], 0), 1);
		i++;
	}
	if (ft_is_starts_with_digit(exec))
		return (1);
	return (0);
}

void	ft_export_add_key_value(t_exec *exec, t_state *s, int i, int j)
{
	char	*key;
	char	*value;

	value = NULL;
	key = ft_substr(exec->cmd_args[i], 0, j, s);
	if (exec->cmd_args[i][j] == '=')
		value = ft_substr(exec->cmd_args[i], j + 1, \
			ft_strlen(exec->cmd_args[i]) - j - 1, s);
	if (ft_arr_include(s->env, key, ft_env_key_cmp) == -1)
		s->env = ft_add_env(s->env, key, value, s);
	else
	{
		j = ft_arr_include(s->env, key, ft_env_key_cmp);
		free(s->env[j]);
		s->env[j] = ft_strjoin(key, "=", s);
		s->env[j] = ft_strjoin(s->env[j], value, NULL);
	}
}

void	ft_print_exp(char **env)
{
	int	i;

	i = -1;
	while (env[++i])
		printf("declare -x %s\n", env[i]);
}

int	ft_export(t_exec *exec, t_state *s)
{
	int		i;
	int		j;

	if (!exec->cmd_args[1])
		return (ft_print_exp(s->env), 0);
	if (ft_export_is_valid(exec))
		return (1);
	i = 1;
	while (exec->cmd_args[i])
	{
		j = 0;
		while (exec->cmd_args[i][j] && exec->cmd_args[i][j] != '=')
			j++;
		ft_export_add_key_value(exec, s, i, j);
		i++;
	}
	return (0);
}
