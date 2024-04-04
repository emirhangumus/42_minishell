/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_err.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: burkaya <burkaya@student.42istanbul.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/02 21:17:18 by egumus            #+#    #+#             */
/*   Updated: 2024/04/04 22:08:48 by burkaya          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_set_exec_err(t_exec *exec, int err, char *value)
{
	if (exec->err_no)
		return ;
	exec->err_no = err;
	exec->err_value = value;
}

void	ft_print_exec_errors(t_state *s, t_exec **exec)
{
	int	i;

	i = 0;
	while (i < s->cmd_amount)
	{
		if (exec[i]->err_no)
			ft_error(exec[i]->err_no, exec[i]->err_value, 0);
		i++;
	}
}
