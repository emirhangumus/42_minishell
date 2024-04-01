/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cmds1.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: egumus <egumus@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/09 17:00:37 by burkaya           #+#    #+#             */
/*   Updated: 2024/04/01 01:29:33 by egumus           ###   ########.fr       */
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
	int		index;
	char	*key;

	i = 1;
	while (exec->cmd_args[i])
	{
		j = 0;
		while (exec->cmd_args[i][j] && exec->cmd_args[i][j] != '=')
			j++;
		key = ft_substr(exec->cmd_args[i], 0, j, s);
		index = ft_arr_include(s->env, key, ft_env_key_cmp);
		if (index != -1)
			ft_arr_remove_by_index(&s->env, index, s);
		i++;
	}
	return (0);
}

int	ft_exit(t_exec *exec, t_state *s)
{
	int	exit_code;
	int	cmd_args_amount;

	cmd_args_amount = ft_arr_len(exec->cmd_args);
	printf("exit\n");
	if (cmd_args_amount > 2)
	{
		write(2, "exit: too many arguments\n", 26);
		exit_code = 1;
	}
	else
	{
		if (exec->cmd_args[1])
			exit_code = ft_atoi(exec->cmd_args[1]);
		else
			exit_code = 0;
	}
	if (exec->cmd_args[1] && ft_isallnum(exec->cmd_args[1]) == 0)
	{
		ft_error(ERR_NUMERIC_ARG, exec->cmd_args[1], 0);
		exit_code = 255;
	}
	s->exit_status = &exit_code;
	exit(exit_code);
	return (0);
}

int	ft_cd(t_exec *exec, t_state *s)
{
	struct stat	buf;
	int			j;

	stat(exec->cmd_args[1], &buf);
	if (!S_ISDIR(buf.st_mode))
		return (ft_error(ERR_NOT_A_DIRECTORY, exec->cmd_args[1], 0), 1);
	if (exec->cmd_args[1] == NULL)
	{
		if (chdir(ft_get_env(s->env, "HOME")) == -1)
			write(2, "cd: HOME not set\n", 16);
	}
	else if (ft_strcmp(exec->cmd_args[1], "-") == 0)
	{
		if (chdir(ft_get_env(s->env, "OLDPWD")) == -1)
			write(2, "cd: OLDPWD not set\n", 19);
		j = ft_arr_include(s->env, "OLDPWD", ft_env_key_cmp);
		free(s->env[j]);
		s->env[j] = ft_strjoin("OLDPWD", "=", s);
		s->env[j] = ft_strjoin(s->env[j], s->cwd, NULL);
	}
	else if (chdir(exec->cmd_args[1]) == -1)
		return (ft_error(ERR_NO_FILE_OR_DIR, exec->cmd_args[1], 0), 1);
	exec->cmd_args[1] = ft_strdup(ft_strjoin("OLDPWD=", s->cwd, s), s);
	s->cwd = getcwd(NULL, 0);
	return (ft_export(exec, s), 0);
}

int	ft_pwd(t_state *s)
{
	printf("%s\n", s->cwd);
	return (0);
}
