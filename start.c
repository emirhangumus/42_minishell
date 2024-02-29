/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   start.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: burkaya <burkaya@student.42istanbul.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/18 19:42:03 by egumus            #+#    #+#             */
/*   Updated: 2024/02/29 14:49:50 by burkaya          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*ft_get_prompt_text(t_state *s)
{
	if (s->status == 0)
		return (ft_strjoin(COLOR_GREEN"MINISHELL"COLOR_RESET":"COLOR_BLUE, ft_strjoin(s->cwd, COLOR_RESET"$ ", s), s));
	else
		return (ft_strjoin(COLOR_RED"MINISHELL"COLOR_RESET":"COLOR_BLUE, ft_strjoin(s->cwd, COLOR_RESET"$ ", s), s));
}

void	ft_start(t_state *s)
{	
	while (1)
	{
		s->cmd = readline(ft_get_prompt_text(s));
		if (!s->cmd)
			break ;
		if (ft_strlen(s->cmd) > 0)
			add_history(ft_strdup(s->cmd, s));
		ft_lexer(s);
        ft_execuator(s);
        ft_free_tokens(s->tokens);
		// printf("tokens:\n");
		// while (s->tokens)
		// {
		// 	printf("type: %d, value: %s\n", s->tokens->type, s->tokens->value);
		// 	s->tokens = s->tokens->next;
		// }
	}
	ft_free_garbage(s);
}
