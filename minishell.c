/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: egumus <egumus@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/18 19:04:05 by egumus            #+#    #+#             */
/*   Updated: 2024/04/01 01:05:52 by egumus           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**ft_init_env(t_state *s)
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
		env[i] = ft_strdup(environ[i], s);
		if (!env[i])
			return (NULL);
		i++;
	}
	env[i] = NULL;
	ft_add_garbage(s, env);
	return (env);
}

int	ft_init_state(t_state *s, int ac, char **av)
{
	if (ac != 1)
	{
		write(2, "minishell: too many arguments\n", 30);
		return (1);
	}
	s->pipes = NULL;
	s->forks = NULL;
	s->garbage = NULL;
	s->cmd = NULL;
	s->tokens = NULL;
	s->cwd = getcwd(NULL, 0);
	s->status = 0;
	s->cmd_amount = 0;
	s->exit_status = NULL;
	s->env = ft_init_env(s);
	s->exec_name = ft_strdup(ft_strrchr(av[0], '/') + 1, s);
	return (0);
}

int	main(int ac, char **av)
{
	t_state	*s;
	int		exit_status;

	s = (t_state *)malloc(sizeof(t_state));
	if (!s)
		return (1);
	if (ft_init_state(s, ac, av))
		return (free(s), 1);
	ft_start(s);
	ft_free_garbage(s);
	exit_status = s->status;
	free(s);
	return (exit_status);
}

// __attribute__((destructor))
// static void test() {
// 	system("leaks minishell");
// }
