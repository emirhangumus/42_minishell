/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_add_env.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: burkaya <burkaya@student.42istanbul.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/26 15:10:54 by burkaya           #+#    #+#             */
/*   Updated: 2024/03/26 15:11:11 by burkaya          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	**ft_add_env(char **env, char *key, char *value, t_state *s)
{
	int		i;
	char	*new_env;
	char	**new_envp;

	i = 0;
	while (env[i])
		i++;
	new_env = ft_strjoin(key, "=", s);
	new_env = ft_strjoin(new_env, value, NULL);
	new_envp = (char **)malloc(sizeof(char *) * (i + 2));
	i = 0;
	while (env[i])
	{
		new_envp[i] = ft_strdup(env[i], NULL);
		i++;
	}
	new_envp[i] = new_env;
	new_envp[i + 1] = NULL;
	ft_free_tab(env);
	return (new_envp);
}
