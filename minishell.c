/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: egumus <egumus@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/14 22:59:40 by egumus            #+#    #+#             */
/*   Updated: 2024/02/15 06:16:53 by egumus           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_init_state(t_state *s)
{
	s->garbage = NULL;
	s->tokens = NULL;
	s->env = NULL;
	ft_make_env(s);
	return (0);
}

int main(int ac, char **av)
{
	(void)ac;
	(void)av;
	t_state	*state;
	char *s;

	state = malloc(sizeof(t_state));
	if (!state)
		return (1);
	if (ft_init_state(state))
		return (1);
	if (ft_make_env(state))
		return (1);
	printf("%s\n", getcwd(NULL, 0));
	while (1)
	{
		s = readline("Sexy Minishell -> ");
		add_history(s);
		int pid = fork();
		if (pid == 0)
		{
			ft_parse(state, s);
			ft_exec(state);
			ft_free_tokens(&state->tokens);
			ft_free_garbage(&state->garbage);
			kill(getpid(), SIGKILL);
		}
		else
			waitpid(pid, NULL, 0);
		// rl_clear_history();
	}
}
