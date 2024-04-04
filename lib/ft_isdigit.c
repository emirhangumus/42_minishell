/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isdigit.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: burkaya <burkaya@student.42istanbul.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/03 15:31:35 by egumus            #+#    #+#             */
/*   Updated: 2024/04/04 22:11:39 by burkaya          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	ft_isdigit(int c)
{
	if (c >= '0' && c <= '9')
		return (1);
	return (SUCCESS);
}

int	ft_is_starts_with_digit(char *value)
{
	if (ft_isdigit(value[0]))
	{
		ft_error(ERR_NOT_VALID_IDFR, value, 0);
		return (1);
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

int	ft_is_valid_env_key_char(char c)
{
	if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')
		|| c == '_' || (c >= '0' && c <= '9'))
		return (1);
	return (SUCCESS);
}

int	ft_isalnum(int c)
{
	if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_')
		return (1);
	return (SUCCESS);
}
