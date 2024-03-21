/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cmds.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: egumus <egumus@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/09 17:00:37 by burkaya           #+#    #+#             */
/*   Updated: 2024/03/22 02:05:24 by egumus           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_env(t_state *s)
{
	int	i;

	i = 0;
	while (s->env[i])
	{
		printf("%s\n", s->env[i]);
		i++;
	}
	return (0);
}

int	ft_unset(t_exec *exec, t_state *s)
{
	int		i;
	int		j;
	int		k;
	char	**new_env;
	char	*key;

	i = 1;
	while (exec->cmd_args[i])
	{
		j = 0;
		while (exec->cmd_args[i][j] && exec->cmd_args[i][j] != '=')
			j++;
		key = ft_substr(exec->cmd_args[i], 0, j, s);
		new_env = (char **)malloc(sizeof(char *) * ft_arr_len(s->env));
		j = 0;
		k = 0;
		while (s->env[j])
		{
			if (ft_strncmp(s->env[j], key, ft_strlen(key)) != 0)
			{
				new_env[k] = ft_strdup(s->env[j], s);
				k++;
			}
			j++;
		}
		new_env[k] = NULL;
		s->env = new_env;
		i++;
	}
	return (0);
}

int ft_isallnum(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (ft_isdigit(str[i]) == 0)
			return (0);
		i++;
	}
	return (1);
}

int	ft_exit(t_exec *exec, t_state *s)
{
	int	exit_code;
	int	cmd_args_amount;

	cmd_args_amount = ft_arr_len(exec->cmd_args);
	printf("exit\n");
	if (cmd_args_amount > 2)
		ft_write_error("exit", "too many arguments");
	if (exec->cmd_args[1])
		exit_code = ft_atoi(exec->cmd_args[1]);
	else
		exit_code = 0;
	if (ft_isallnum(exec->cmd_args[1]) == 0)
	{
		ft_write_error("exit", "numeric argument required");
		exit(255);
	}
	s->exit_status = &exit_code;
	return (SYS_EXIT);
}

int	ft_cd(t_exec *exec, t_state *s)
{
	if (exec->cmd_args[1] == NULL)
	{
		if (chdir(ft_get_env(s->env, "HOME")) == -1)
			printf("cd: HOME not set\n");
	}
	else if (ft_strcmp(exec->cmd_args[1], "~") == 0)
	{
		if (chdir(ft_get_env(s->env, "HOME")) == -1)
			printf("cd: HOME not set\n");
	}
	else if (ft_strcmp(exec->cmd_args[1], "-") == 0)
	{
		if (chdir(ft_get_env(s->env, "OLDPWD")) == -1)
			printf("cd: OLDPWD not set\n");
	}
	else if (chdir(exec->cmd_args[1]) == -1)
	{
		printf("cd: %s: No such file or directory\n", exec->cmd_args[1]);
		return (1);
	}
	s->cwd = getcwd(s->cwd, 1024);
	return (0);
}

int	ft_pwd(t_state *s)
{
	printf("%s\n", s->cwd);
	return (0);
}
