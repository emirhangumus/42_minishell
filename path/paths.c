/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   paths.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: egumus <egumus@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/14 23:00:41 by egumus            #+#    #+#             */
/*   Updated: 2024/02/15 04:03:46 by egumus           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	ft_free_env(t_state *s)
{
	int	i;

	i = 0;
	while (s->env[i])
	{
		if (ft_strcmp(s->env[i]->key, "PATH") == 0)
			ft_free_tab(s->env[i]->value);
		else
			free(s->env[i]->value);
		free(s->env[i]->key);
		free(s->env[i]);
		i++;
	}
	free(s->env);
}

int		ft_make_env(t_state *s)
{
	extern char	**environ;
	int			i;
	char		**tmp;
	t_map		*map;

	i = 0;
	if (s->env)
		ft_free_env(s);
	s->env = malloc(sizeof(t_map *) * (ft_tab_len(environ) + 1));
	if (!s->env)
		return (1);
	s->env[0] = NULL;
	i = 0;
	while (environ[i])
	{
		tmp = ft_split(environ[i], '=');
		map = malloc(sizeof(t_map));
		if (!map || !tmp)
			return (1);
		map->key = ft_strdup(tmp[0]);
		if (ft_strcmp(tmp[0], "PATH") == 0)
			map->value = ft_split(tmp[1], ':');
		else
			map->value = ft_strdup(tmp[1]);
		s->env[i] = map;
		free(tmp[0]);
		free(tmp[1]);
		free(tmp);
		i++;
	}
	s->env[i] = NULL;
	return (0);
}