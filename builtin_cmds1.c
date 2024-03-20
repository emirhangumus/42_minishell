/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cmds1.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: burkaya <burkaya@student.42istanbul.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/09 17:00:46 by burkaya           #+#    #+#             */
/*   Updated: 2024/03/20 09:56:08 by burkaya          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**ft_add_env(char **env, char *key, char *value, t_state *s)
{
	int		i;
	char	*new_env;
	char	**new_envp;

	i = 0;
	while (env[i])
		i++;
	new_env = ft_strjoin(key, "=", s);
	new_env = ft_strjoin(new_env, value, NULL);
	new_envp = (char **)malloc(sizeof(char *) * (i + 2));
	i = 0;
	while (env[i])
	{
		new_envp[i] = ft_strdup(env[i], NULL);
		i++;
	}
	new_envp[i] = new_env;
	new_envp[i + 1] = NULL;
	ft_free_tab(env);
	return (new_envp);
}

int	ft_env_key_cmp(const char *s1, const char *s2)
{
	int	i;

	i = 0;
	while (s1[i] && s2[i] && s1[i] == s2[i])
		i++;
	if (s1[i] == '=' && s2[i] == '\0')
		return (0);
	return (1);
}

int	ft_export(t_exec *exec, t_state *s)
{
	int		i;
	int		j;
	char	*key;
	char	*value;

	i = 1;
	while (exec->cmd_args[i])
	{
		j = 0;
		while (exec->cmd_args[i][j] && exec->cmd_args[i][j] != '=')
			j++;
		key = ft_substr(exec->cmd_args[i], 0, j, s);
		if (exec->cmd_args[i][j] == '=')
			value = ft_substr(exec->cmd_args[i], j + 1, \
				ft_strlen(exec->cmd_args[i]) - j - 1, s);
		else
			value = NULL;
		if (ft_arr_include(s->env, key, ft_env_key_cmp) == -1)
			s->env = ft_add_env(s->env, key, value, s);
		else
		{
			j = ft_arr_include(s->env, key, ft_env_key_cmp);
			free(s->env[j]);
			s->env[j] = ft_strjoin(key, "=", s);
			s->env[j] = ft_strjoin(s->env[j], value, NULL);
		}
		i++;
	}
	return (0);
}

int	ft_echo_is_valid(char *arg)
{
	int	i;

	i = 0;
	if (arg[i] == '-')
		i++;
	if (arg[i] == 'n')
	{
		i++;
		while (arg[i] == 'n')
			i++;
		if (arg[i] == '\0')
			return (1);
	}
	return (0);
}

int	ft_echo(t_exec *exec)
{
	int	i;
	int	n_flag;

	i = 1;
	n_flag = 0;
	while (exec->cmd_args[i] && ft_echo_is_valid(exec->cmd_args[i]))
	{
		n_flag = 1;
		i++;
	}
	while (exec->cmd_args[i])
	{
		write(STDOUT_FILENO, exec->cmd_args[i], ft_strlen(exec->cmd_args[i]));
		if (exec->cmd_args[i + 1])
			write(STDOUT_FILENO, " ", 1);
		i++;
	}
	if (!n_flag)
		write(STDOUT_FILENO, "\n", 1);
	return (0);
}
