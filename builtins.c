/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: egumus <egumus@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/01 02:43:20 by egumus            #+#    #+#             */
/*   Updated: 2024/03/05 01:47:49 by egumus           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_is_builtin(char *value)
{
	// ft_strncmp kullanmadığımız için
	// fazladan echoo gibi bir komutu da builtin olarak algılayabilir
	if (ft_strcmp(value, "echo") == 0)
		return (1);
	if (ft_strcmp(value, "cd") == 0)
		return (1);
	if (ft_strcmp(value, "pwd") == 0)
		return (1);
	if (ft_strcmp(value, "export") == 0)
		return (1);
	if (ft_strcmp(value, "unset") == 0)
		return (1);
	if (ft_strcmp(value, "env") == 0)
		return (1);
	if (ft_strcmp(value, "exit") == 0)
		return (1);
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
	int i;
	int j;
	int k;
	char **new_env;
	char *key;

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
	int	i;

	i = 0;
	if (exec->cmd_args[1])
	{
		while (exec->cmd_args[1][i])
		{
			if (ft_isdigit(exec->cmd_args[1][i]) == 0)
			{
				printf("exit\n");
				printf("minishell: exit: %s: numeric argument required\n", \
					exec->cmd_args[1]);
				exit(255);
			}
			i++;
		}
		exit(ft_atoi(exec->cmd_args[1]));
	}
	exit(s->status);
}

int	ft_execute_builtin(t_exec *exec, t_state *s, int pipefd[2])
{
	(void)pipefd;
	if (ft_strcmp(exec->cmd_args[0], "echo") == 0)
		return (ft_echo(exec));
	if (ft_strcmp(exec->cmd_args[0], "cd") == 0)
		return (ft_cd(exec, s));
	if (ft_strcmp(exec->cmd_args[0], "pwd") == 0)
		return (ft_pwd(s));
	if (ft_strcmp(exec->cmd_args[0], "export") == 0)
		return (ft_export(exec, s));
	if (ft_strcmp(exec->cmd_args[0], "unset") == 0)
		return (ft_unset(exec, s));
	if (ft_strcmp(exec->cmd_args[0], "env") == 0)
		return (ft_env(s));
	if (ft_strcmp(exec->cmd_args[0], "exit") == 0)
		return (ft_exit(exec, s));
	return (0);
}
