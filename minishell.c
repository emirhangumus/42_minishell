/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: egumus <egumus@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/18 19:04:05 by egumus            #+#    #+#             */
/*   Updated: 2024/03/03 16:35:50 by egumus           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**ft_init_env(void)
{
	extern char	**environ;
	char		**env;
	int			i;

	i = 0;
	while (environ[i])
		i++;
	env = (char **)malloc(sizeof(char *) * (i + 1));
	if (!env)
		return (NULL);
	i = 0;
	while (environ[i])
	{
		env[i] = ft_strdup(environ[i], NULL);
		if (!env[i])
			return (NULL);
		i++;
	}
	env[i] = NULL;
	return (env);
}

int	ft_init_state(t_state *s)
{
	s->garbage = NULL;
	s->env = ft_init_env();
	s->cmd = NULL;
	s->tokens = NULL;
	s->cwd = (char *)malloc(1024);
	getcwd(s->cwd, 1024);
	s->status = 0;
	return (0);
}

void	ft_sigint(int sig)
{
	(void)sig;
	write(1, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

void	ft_sigquit(int sig)
{
	(void)sig;
}

void	ft_signals(void)
{
	signal(SIGINT, ft_sigint);
	signal(SIGQUIT, ft_sigquit);
}


int	main(void)
{
	t_state	*s;

	s = (t_state *)malloc(sizeof(t_state));
	if (!s)
		return (1);
	if (ft_init_state(s))
		return (free(s), 1);
	ft_signals();
	ft_start(s);
	ft_free_tab(s->env);
	free(s->cwd);
	free(s);
	return (0);
}

// __attribute__((destructor))
// static void test() {
// 	system("leaks minishell");
// }
