/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: burkaya <burkaya@student.42istanbul.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/01 02:43:20 by egumus            #+#    #+#             */
/*   Updated: 2024/03/19 22:21:32 by burkaya          ###   ########.fr       */
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
	return (SUCCESS);
}

int	ft_execute_builtin(t_state *s, t_exec *exec)
{
	if (ft_strcmp(exec->cmd_args[0], "echo") == 0)
		return (ft_echo(exec));
	else if (ft_strcmp(exec->cmd_args[0], "cd") == 0)
		return (ft_cd(exec, s));
	else if (ft_strcmp(exec->cmd_args[0], "pwd") == 0)
		return (ft_pwd(s));
	else if (ft_strcmp(exec->cmd_args[0], "export") == 0)
		return (ft_export(exec, s));
	else if (ft_strcmp(exec->cmd_args[0], "unset") == 0)
		return (ft_unset(exec, s));
	else if (ft_strcmp(exec->cmd_args[0], "env") == 0)
		return (ft_env(s));
	else if (ft_strcmp(exec->cmd_args[0], "exit") == 0)
		return (ft_exit(exec, s));
	return (127);
}
