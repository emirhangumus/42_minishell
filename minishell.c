/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: egumus <egumus@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/18 19:04:05 by egumus            #+#    #+#             */
/*   Updated: 2024/03/05 01:36:40 by egumus           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int g_sig = -1;

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

int	main(void)
{
	t_state	*s;

	s = (t_state *)malloc(sizeof(t_state));
	if (!s)
		return (1);
	if (ft_init_state(s))
		return (free(s), 1);
	ft_start(s);
	ft_free_tab(s->env);
	ft_free_garbage(s);
	free(s->cwd);
	free(s);
	return (0);
}

__attribute__((destructor))
static void test() {
	system("leaks minishell");
}
