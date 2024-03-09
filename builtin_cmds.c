/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cmds.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: burkaya <burkaya@student.42istanbul.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/09 17:00:37 by burkaya           #+#    #+#             */
/*   Updated: 2024/03/09 17:45:51 by burkaya          ###   ########.fr       */
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

int	ft_exit(t_exec *exec, t_state *s)
{
	unsigned char	exit_code;
	int				cmd_args_amount;

	(void)s;
	cmd_args_amount = ft_arr_len(exec->cmd_args);
	if (cmd_args_amount > 2)
	{
		printf("exit\n");
		printf("minishell: exit: too many arguments\n");
		return (1);
	}
	exit_code = ft_atoi(exec->cmd_args[1]);
	if (exit_code == 255)
	{
		printf("exit\n");
		printf("minishell: exit: %s: numeric argument required\n", \
			exec->cmd_args[1]);
		exit(255);
	}
	exit(exit_code);
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
