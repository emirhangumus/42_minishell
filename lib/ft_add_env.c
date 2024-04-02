/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_add_env.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: egumus <egumus@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/26 15:10:54 by burkaya           #+#    #+#             */
/*   Updated: 2024/04/03 00:10:15 by egumus           ###   ########.fr       */
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
	new_env = ft_strjoin(new_env, value, s);
	new_envp = (char **)malloc(sizeof(char *) * (i + 2));
	i = 0;
	while (env[i])
	{
		new_envp[i] = ft_strdup(env[i], s);
		i++;
	}
	new_envp[i] = new_env;
	new_envp[i + 1] = NULL;
	ft_add_garbage(s, new_envp);
	return (new_envp);
}
