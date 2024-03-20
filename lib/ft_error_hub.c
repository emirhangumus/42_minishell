/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_error_hub.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: burkaya <burkaya@student.42istanbul.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/20 10:25:01 by burkaya           #+#    #+#             */
/*   Updated: 2024/03/20 13:49:34 by burkaya          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	ft_error_hub(t_state *s, int err)
{
	(void)s;
	(void)err;
	// if (err == ERR_CMD_NOT_FOUND)
	// {
	// 	ft_write_error("minishell: command not found\n", 2);
	// 	s->status = 127;
	// }
	// else if (err == ERR_NO_FILE_DIR)
	// {
	// 	ft_write_error("minishell: no such file or directory\n", 2);
	// 	s->status = 126;
	// }
}

void	ft_write_error(char *cmd, char *error)
{
	write(STDERR_FILENO, "minishell: ", 11);
	write(STDERR_FILENO, cmd, ft_strlen(cmd));
	write(STDERR_FILENO, ": ", 2);
	write(STDERR_FILENO, error, ft_strlen(error));
	write(STDERR_FILENO, "\n", 1);
}