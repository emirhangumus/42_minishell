/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: egumus <egumus@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/01 02:43:20 by egumus            #+#    #+#             */
/*   Updated: 2024/03/01 10:00:44 by egumus           ###   ########.fr       */
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

int ft_echo(t_exec *exec, int pipefd[2])
{
	int i;
	int n_flag;
	
	(void)pipefd;

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

char **ft_add_env(char **env, char *key, char *value, t_state *s)
{
	int i;
	char *new_env;
	char **new_envp;

	i = 0;
	while (env[i])
		i++;
	new_env = ft_strjoin(key, "=", s);
	if (value)
		new_env = ft_strjoin(new_env, value, s);
	new_envp = (char **)malloc(sizeof(char *) * (i + 2));
	i = 0;
	while (env[i])
	{
		new_envp[i] = ft_strdup(env[i], s);
		i++;
	}
	new_envp[i] = new_env;
	new_envp[i + 1] = NULL;
	return (new_envp);
}

int	ft_export(t_exec *exec, t_state *s)
{
	int i;
	int j;
	char *key;
	char *value;

	i = 1;
	while (exec->cmd_args[i])
	{
		j = 0;
		while (exec->cmd_args[i][j] && exec->cmd_args[i][j] != '=')
			j++;
		key = ft_substr(exec->cmd_args[i], 0, j, s);
		if (exec->cmd_args[i][j] == '=')
			value = ft_substr(exec->cmd_args[i], j + 1, ft_strlen(exec->cmd_args[i]) - j - 1, s);
		else
			value = NULL;
		if (value)
		{
			ft_add_garbage(s, key);
			ft_add_garbage(s, value);
			ft_add_garbage(s, exec->cmd_args[i]);
			ft_add_garbage(s, s->env);
			s->env = ft_add_env(s->env, key, value, s);
		}
		else
		{
			ft_add_garbage(s, key);
			ft_add_garbage(s, exec->cmd_args[i]);
			ft_add_garbage(s, s->env);
			s->env = ft_add_env(s->env, key, NULL, s);
		}
		i++;
	}
	return (0);
}

int	ft_env(t_state *s)
{
	int i;

	i = 0;
	while (s->env[i])
	{
		printf("%s\n", s->env[i]);
		i++;
	}
	return (0);
}

// int	ft_unset(t_exec *exec, t_state *s)
// {
// 	int i;
// 	int j;
// 	int k;
// 	char **new_env;
// 	char *key;

// 	i = 1;
// 	while (exec->cmd_args[i])
// 	{
// 		j = 0;
// 		while (exec->cmd_args[i][j] && exec->cmd_args[i][j] != '=')
// 			j++;
// 		key = ft_substr(exec->cmd_args[i], 0, j, s);
// 		k = 0;
// 		while (s->env[k])
// 		{
// 			if (ft_strncmp(s->env[k], key, j) == 0 && s->env[k][j] == '=')
// 			{
// 				new_env = (char **)malloc(sizeof(char *) * ft_strlen(s->env));
// 				ft_add_garbage(s, new_env);
// 				j = 0;
// 				while (s->env[j])
// 				{
// 					if (j != k)
// 					{
// 						new_env[j] = ft_strdup(s->env[j], s);
// 						ft_add_garbage(s, new_env[j]);
// 					}
// 					j++;
// 				}
// 				new_env[j] = NULL;
// 				ft_add_garbage(s, s->env);
// 				s->env = new_env;
// 				break ;
// 			}
// 			k++;
// 		}
// 		i++;
// 	}
// 	return (0);
// }


int   ft_execute_builtin(t_exec *exec, t_state *s, int pipefd[2])
{
	(void)s;

	if (ft_strcmp(exec->cmd_args[0], "echo") == 0)
		return (ft_echo(exec, pipefd));
	if (ft_strcmp(exec->cmd_args[0], "cd") == 0)
		return (ft_cd(exec, s));
	if (ft_strcmp(exec->cmd_args[0], "pwd") == 0)
		return (ft_pwd(s));
	if (ft_strcmp(exec->cmd_args[0], "export") == 0)
		return (ft_export(exec, s));
	// if (ft_strcmp(exec->cmd_args[0], "unset") == 0)
	// 	return (ft_unset(exec, s));
	if (ft_strcmp(exec->cmd_args[0], "env") == 0)
		return (ft_env(s));
	// if (ft_strcmp(start_token->value, "exit") == 0)
	// 	return (ft_exit(start_token, s));
	return (0);
}