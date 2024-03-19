/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   start.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: burkaya <burkaya@student.42istanbul.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/18 19:42:03 by egumus            #+#    #+#             */
/*   Updated: 2024/03/19 20:16:34 by burkaya          ###   ########.fr       */
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

void	ft_init_tokens(t_state *s)
{
	int pipes;
	
	pipes = ft_count_pipes(s->cmd);
	s->tokens = (t_token **)malloc(sizeof(t_token *) * (pipes + 2));
	if (!s->tokens)
		return ;
	pipes += 1;
	while (pipes >= 0)
	{
		s->tokens[pipes] = NULL;
		pipes--;
	}
}

void	ft_start(t_state *s)
{
	int		err;
	
	ft_signals();
	while (1)
	{
		if (s->cmd)
			free(s->cmd);
		s->cmd = readline(ft_get_prompt_text(s));
		if (!s->cmd)
		{
			write(1, "\033[2Dexit\n", 8);
			ft_free_tokens(s->tokens);
			break;
		}
		if (ft_strlen(s->cmd) > 0 && ft_isfullof(s->cmd, ' ') == 0)
		{
			add_history(s->cmd);
			ft_init_tokens(s);
			err = ft_lexer(s);
			if (err != 0)
			{
				ft_free_tokens(s->tokens);
				s->tokens = NULL;
				continue ;
			}
			ft_execuator(s);
			// ft_print_tokens(s->tokens);
			// if (s->tokens != NULL && ft_execuator(s))
			// {
			// 	printf("minishell: [duzenlenecek]: command not found\n");
			// }
			ft_free_tokens(s->tokens);
			s->tokens = NULL;
		}
	}
}
