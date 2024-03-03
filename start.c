/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   start.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: egumus <egumus@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/18 19:42:03 by egumus            #+#    #+#             */
/*   Updated: 2024/03/03 21:53:06 by egumus           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern int	g_sig;

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
	while (1)
	{
		if (s->cmd)
			free(s->cmd);
		s->cmd = readline(ft_get_prompt_text(s));
		if (s->cmd && ft_strlen(s->cmd) > 0 && ft_isfullof(s->cmd, ' ') == 0)
		{
			add_history(ft_strdup(s->cmd, s));
			ft_lexer(s);
			if (ft_execuator(s))
			{
				printf("minishell: an error accured %s\n", s->tokens->value);
				s->status = ERR_CMD_NOT_FOUND;
			}
			ft_free_tokens(s->tokens);
			s->tokens = NULL;
		}
	}
	ft_free_garbage(s);
}
