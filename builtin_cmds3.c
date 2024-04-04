/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cmds3.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: burkaya <burkaya@student.42istanbul.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/09 17:00:46 by burkaya           #+#    #+#             */
/*   Updated: 2024/04/04 23:25:57 by burkaya          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_exp_uns_is_valid(char *value, int is_export)
{
	int	j;
	int	flag;

	flag = 0;
	j = 0;
	if (value[j] == 0 || (is_export && (value[j] == '=' || value[j] == '+')))
		return (ft_error(ERR_NOT_VALID_IDFR, value, 0), 1);
	if (ft_is_starts_with_digit(value))
		return (1);
	while (value[j])
	{
		if (is_export && value[j] == '=')
			break ;
		if (!ft_is_valid_env_key_char(value[j]) || value[j] == '-')
		{
			if (!(is_export && value[j] == '+' && value[j + 1] == '='))
				return (ft_error(ERR_NOT_VALID_IDFR, \
						value, 0), 1);
		}
		j++;
	}
	if (is_export && !value[j])
		return (2);
	return (0);
}

void	ft_export_add_key_value(char *str, t_state *s, int j)
{
	char	*key;
	char	*value;

	value = NULL;
	key = ft_substr(str, 0, j, s);
	if (str[j - 1] == '+')
		key = ft_substr(str, 0, j - 1, s);
	if (str[j] == '=')
	{
		value = ft_substr(str, j + 1, \
			ft_strlen(str) - j - 1, s);
		if (str[j - 1] == '+' && ft_get_env(s->env, key))
			value = ft_strjoin(ft_get_env(s->env, key), value, s);
	}
	if (ft_arr_include(s->env, key, ft_env_key_cmp) == -1)
		s->env = ft_add_env(s->env, key, value, s);
	else
	{
		j = ft_arr_include(s->env, key, ft_env_key_cmp);
		s->env[j] = ft_strjoin(key, "=", s);
		s->env[j] = ft_strjoin(s->env[j], value, s);
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
	int		ret;
	int		flag;

	flag = 0;
	i = 0;
	if (!exec->cmd_args[1])
		return (ft_print_exp(s->env), 0);
	while (exec->cmd_args[++i])
	{
		ret = ft_exp_uns_is_valid(exec->cmd_args[i], 1);
		if (ret == 2)
			continue ;
		else if (ret == 1)
		{
			flag = ret;
			continue ;
		}
		j = 0;
		while (exec->cmd_args[i][j] && exec->cmd_args[i][j] != '=')
			j++;
		ft_export_add_key_value(exec->cmd_args[i], s, j);
	}
	return (flag);
}
