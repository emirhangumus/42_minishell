/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_fopen.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: burkaya <burkaya@student.42istanbul.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/30 13:56:59 by burkaya           #+#    #+#             */
/*   Updated: 2024/03/30 14:07:03 by burkaya          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	ft_for_in_file(t_exec *exec)
{
	if (!access(exec->in_file, F_OK) && access(exec->in_file, R_OK) == -1)
	{
		exec->should_run = 1;
		if (!exec->is_here_doc)
			ft_error(ERR_PERMISSION_DENIED, exec->in_file, 0);
		return (126);
	}
	exec->should_run = 1;
	if (exec->is_here_doc)
		return (0);
	ft_error(ERR_NO_FILE_OR_DIR, exec->in_file, 0);
	return (ERR_NO_FILE_OR_DIR);
}

static int	ft_for_out_file(t_exec *exec)
{
	if (!access(exec->out_file, F_OK) && access(exec->out_file, W_OK) == -1)
	{
		exec->should_run = 1;
		ft_error(ERR_PERMISSION_DENIED, exec->out_file, 0);
		return (1);
	}
	exec->should_run = 1;
	ft_error(ERR_NO_FILE_OR_DIR, exec->out_file, 0);
	return (ERR_NO_FILE_OR_DIR);
}

int	ft_open_check_files(t_exec *exec, int status)
{
	if (exec->in_file)
	{
		if (exec->in_type == T_LREDIR && exec->should_run == 0)
			exec->in_fd = open(exec->in_file, O_RDONLY);
		else if (exec->in_type == T_LAPPEND)
			return (status);
		if (exec->in_fd == -1 && exec->should_run == 0)
			return (ft_for_in_file(exec));
	}
	if (exec->out_file && exec->should_run == 0)
	{
		if (exec->out_type == T_RREDIR)
			exec->out_fd = open(exec->out_file, \
				O_WRONLY | O_CREAT | O_TRUNC, 0644);
		else if (exec->out_type == T_RAPPEND)
			exec->out_fd = open(exec->out_file, \
				O_WRONLY | O_CREAT | O_APPEND, 0644);
		if (exec->out_fd == -1 && exec->should_run == 0)
			return (ft_for_out_file(exec));
	}
	return (status);
}
