/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   start.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: egumus <egumus@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/18 19:42:03 by egumus            #+#    #+#             */
/*   Updated: 2024/04/01 01:42:11 by egumus           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*ft_get_prompt_text(t_state *s)
{
	// Remove then push to intra
	return (ft_strjoin(COLOR_GREEN"MINISHELL"COLOR_RESET":"COLOR_BLUE, \
		ft_strjoin(s->cwd, COLOR_RESET"$ ", s), s));
	if (s->status == 0)
		return (ft_strjoin(COLOR_GREEN"MINISHELL"COLOR_RESET":"COLOR_BLUE, \
		ft_strjoin(s->cwd, COLOR_RESET"$ ", s), s));
	else
		return (ft_strjoin(COLOR_RED"MINISHELL"COLOR_RESET":"COLOR_BLUE, \
		ft_strjoin(s->cwd, COLOR_RESET"$ ", s), s));
}

void	ft_init_tokens(t_state *s)
{
	int	pipes;

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

static void	ft_start_run_free(t_state *s)
{
	ft_free_tokens(s->tokens);
	free(s->tokens);
	if (s->pipes)
		free(s->pipes);
	if (s->forks)
		free(s->forks);
	s->pipes = NULL;
	s->forks = NULL;
	s->tokens = NULL;
}

static int	ft_start_run(t_state *s)
{
	int	err;

	if (ft_strlen(s->cmd) > 0 && ft_isfullof(s->cmd, ' ') == 0)
	{
		add_history(s->cmd);
		ft_init_tokens(s);
		err = ft_lexer(s);
		if (err)
		{
			ft_free_tokens(s->tokens);
			free(s->tokens);
			s->tokens = NULL;
			ft_error(err, NULL, 0);
			return (err);
		}
		ft_init_prev_tokens(s->tokens);
		ft_execuator(s);
		ft_start_run_free(s);
	}
	return (SUCCESS);
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
			break ;
		if (ft_start_run(s) != 1)
			continue ;
	}
}
