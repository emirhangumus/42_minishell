/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: burkaya <burkaya@student.42istanbul.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/26 15:20:34 by burkaya           #+#    #+#             */
/*   Updated: 2024/03/26 15:25:06 by burkaya          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_err_arg(char *str, char *arg)
{
	write(2, "minishell: ", 11);
	write(2, arg, ft_strlen(arg));
	write(2, ": ", 2);
	write(2, str, ft_strlen(str));
	write(2, "\n", 1);
}

void	ft_error_part1(int err, char *str)
{
	if (err == ERR_CMD_NOT_FOUND)
	{
		if (str)
			ft_err_arg("command not found", str);
		else
			write(2, "minishell: command not found\n", 27);
	}
	else if (err == ERR_IS_A_DIRECTORY)
	{
		if (str)
			ft_err_arg("is a directory", str);
		else
			write(2, "minishell: is a directory\n", 26);
	}
	else if (err == ERR_PERMISSION_DENIED)
	{
		if (str)
			ft_err_arg("Permission denied", str);
		else
			write(2, "minishell: permission denied\n", 30);
	}
}

void	ft_error_part2(int err, char *str)
{
	if (err == ERR_UNEXPECTED_TOKEN)
	{
		if (str)
			ft_err_arg("syntax error near unexpected token", str);
		else
			write(2, "minishell: syntax error near unexpected token\n", 47);
	}
	else if (err == ERR_EMPTY_COMMAND)
	{
		if (str)
			ft_err_arg("command not found", str);
		else
			write(2, "minishell: command not found\n", 27);
	}
	else if (err == ERR_NUMERIC_ARG)
	{
		if (str)
			ft_err_arg("numeric argument required", str);
		else
			write(2, "minishell: numeric argument required\n", 37);
	}
}

void	ft_error_part3(int err, char *str)
{
	if (err == ERR_PIPE_INIT)
	{
		if (str)
			ft_err_arg("pipe init error", str);
		else
			write(2, "minishell: pipe init error\n", 28);
	}
	else if (err == ERR_MALLOC)
	{
		if (str)
			ft_err_arg("malloc error", str);
		else
			write(2, "minishell: malloc error\n", 25);
	}
	else if (err == ERR_NO_FILE_OR_DIR)
	{
		if (str)
			ft_err_arg("No such file or directory", str);
		else
			write(2, "minishell: No such file or directory\n", 37);
	}
}

void	ft_error(int err, char *str, int throw_exit)
{
	ft_error_part1(err, str);
	ft_error_part2(err, str);
	ft_error_part3(err, str);
	if (err == ERR_NOT_VALID_IDENTIFIER)
	{
		if (str)
			ft_err_arg("not a valid identifier", str);
		else
			write(2, "minishell: not a valid identifier\n", 35);
	}
	else if (throw_exit)
		exit(1);
}

//export: `1a': not a valid identifier