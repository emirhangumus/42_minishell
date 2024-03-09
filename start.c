/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   start.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: burkaya <burkaya@student.42istanbul.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/18 19:42:03 by egumus            #+#    #+#             */
/*   Updated: 2024/03/09 17:23:09 by burkaya          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*ft_get_prompt_text(t_state *s)
{
	if (s->status == 0)
		return (ft_strjoin(COLOR_GREEN"MINISHELL"COLOR_RESET":"COLOR_BLUE, \
		ft_strjoin(s->cwd, COLOR_RESET"$ ", s), s));
	else
		return (ft_strjoin(COLOR_RED"MINISHELL"COLOR_RESET":"COLOR_BLUE, \
		ft_strjoin(s->cwd, COLOR_RESET"$ ", s), s));
}

void	ft_start(t_state *s)
{
	ft_signals();
	while (1)
	{
		if (s->cmd)
			free(s->cmd);
		s->cmd = readline(ft_get_prompt_text(s));
		if (!s->cmd)
		{
			write(1, "\033[2Dexit\n", 8);
			break ;
		}
		if (ft_strlen(s->cmd) > 0 && ft_isfullof(s->cmd, ' ') == 0)
		{
			add_history(s->cmd);
			ft_lexer(s);
			if (s->tokens != NULL && ft_execuator(s))
			{
				printf("minishell: an error accured %s\n", s->tokens->value);
				s->status = ERR_CMD_NOT_FOUND;
			}
			ft_free_tokens(s->tokens);
			s->tokens = NULL;
		}
	}
}
