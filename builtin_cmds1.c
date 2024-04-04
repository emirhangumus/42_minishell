/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cmds1.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: burkaya <burkaya@student.42istanbul.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/09 17:00:37 by burkaya           #+#    #+#             */
/*   Updated: 2024/04/04 19:37:39 by burkaya          ###   ########.fr       */
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
	int		flag;
	int		ret;

	flag = 0;
	i = 0;
	while (exec->cmd_args[++i])
	{
		if (ft_strchr(exec->cmd_args[i], '='))
		{
			ft_error(ERR_NOT_VALID_IDFR, exec->cmd_args[i], 0);
			flag = 1;
			continue ;
		}
		ret = ft_exp_uns_is_valid(exec->cmd_args[i], 0);
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
		key = ft_substr(exec->cmd_args[i], 0, j, s);
		index = ft_arr_include(s->env, key, ft_env_key_cmp);
		if (index != -1)
			ft_arr_remove_by_index(&s->env, index, s);
	}
	return (flag);
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
	if (exec->cmd_args[1] && (ft_isallnum(exec->cmd_args[1]) == 0 || exec->cmd_args[1][0] == 0))
	{
		ft_error(ERR_NUMERIC_ARG, exec->cmd_args[1], 0);
		exit_code = 255;
	}
	s->exit_status = &exit_code;
	exit(exit_code);
	return (0);
}

static void	ft_cd_setter(t_state *s)
{
	ft_export_add_key_value(ft_strdup(ft_strjoin("OLDPWD=", \
		s->cwd, s), s), s, 6);
	s->cwd = getcwd(NULL, 0);
	ft_export_add_key_value(ft_strdup(ft_strjoin("PWD=", s->cwd, s), s), s, 3);
	ft_add_garbage(s, s->cwd);
}

int	ft_cd(t_exec *exec, t_state *s)
{
	struct stat	buf;

	if (exec->cmd_args[1] == NULL)
	{
		if (ft_get_env(s->env, "HOME") == NULL)
			write(2, "cd: HOME not set\n", 16);
		exec->cmd_args[1] = ft_get_env(s->env, "HOME");
	}
	else if (ft_strcmp(exec->cmd_args[1], "-") == 0)
	{
		if (ft_get_env(s->env, "OLDPWD") == NULL)
			write(2, "cd: OLDPWD not set\n", 19);
		exec->cmd_args[1] = ft_get_env(s->env, "OLDPWD");
	}
	stat(exec->cmd_args[1], &buf);
	if (!S_ISDIR(buf.st_mode))
		return (ft_error(ERR_NOT_A_DIRECTORY, exec->cmd_args[1], 0), 1);
	if (exec->cmd_args[1] && chdir(exec->cmd_args[1]) == -1)
		return (ft_error(ERR_NO_FILE_OR_DIR, exec->cmd_args[1], 0), 1);
	return (ft_cd_setter(s), 0);
}
