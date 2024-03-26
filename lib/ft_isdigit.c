/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isdigit.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: burkaya <burkaya@student.42istanbul.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/03 15:31:35 by egumus            #+#    #+#             */
/*   Updated: 2024/03/26 15:12:41 by burkaya          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	ft_isdigit(int c)
{
	if (c >= '0' && c <= '9')
		return (1);
	return (SUCCESS);
}

int	ft_is_starts_with_digit(t_exec *exec)
{
	int	i;

	i = 1;
	while (exec->cmd_args[i])
	{
		if (ft_isdigit(exec->cmd_args[i][0]))
		{
			ft_error(ERR_NOT_VALID_IDENTIFIER, exec->cmd_args[i], 0);
			return (1);
		}
		i++;
	}
	return (0);
}

int	ft_isallnum(char *str)
{
	int	i;

	i = 0;
	if (str[i] == '-')
		i++;
	else if (str[i] == '+')
		i++;
	while (str[i])
	{
		if (ft_isdigit(str[i]) == 0)
			return (0);
		i++;
	}
	return (1);
}
