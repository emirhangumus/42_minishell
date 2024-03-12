/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   start.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: egumus <egumus@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/18 19:42:03 by egumus            #+#    #+#             */
/*   Updated: 2024/03/12 15:40:41 by egumus           ###   ########.fr       */
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
	char	*h;
	char	*err;
	
	ft_signals();
	while (1)
	{
		if (s->cmd)
			free(s->cmd);
		s->cmd = readline(ft_get_prompt_text(s));
		h = s->cmd;
		s->cmd = ft_strtrim(s->cmd, " ", NULL);
		free(h);
		if (!s->cmd)
		{
			write(1, "\033[2Dexit\n", 8);
			exit(s->status);
		}
		if (ft_strlen(s->cmd) > 0 && ft_isfullof(s->cmd, ' ') == 0)
		{
			add_history(s->cmd);
			ft_lexer(s);
			s->tokens->type = T_CMD;
			if (s->tokens != NULL && ft_execuator(s))
			{
				err = ft_strjoin("bash: ", s->tokens->value, s);
				err = ft_strjoin(err, ": command not found\n", s);
				write(2, err, ft_strlen(err));
				s->status = ERR_CMD_NOT_FOUND;
				break ;
			}
			ft_free_tokens(s->tokens);
			s->tokens = NULL;
		}
	}
}
