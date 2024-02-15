/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: egumus <egumus@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/15 05:21:26 by egumus            #+#    #+#             */
/*   Updated: 2024/02/15 06:17:04 by egumus           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char **ft_get_args(t_state *s)
{
	int		i;
	int		j;
	t_token	*current;
	char	**res;

	i = 0;
	j = 0;
	res = malloc(sizeof(char *) * (ft_token_size(s->tokens, T_PARAM) + 1));
	if (!res)
		return (NULL);
	current = s->tokens;
	while (current)
	{
		if (current->type == T_PARAM)
		{
			res[j] = ft_mstrdup(current->value, s);
			j++;
		}
		current = current->next;
	}
	res[j] = NULL;
	return (res);
}


void	ft_exec(t_state *s)
{
	(void)s;
	extern char **environ;
	char **paths;
	
	paths = ((char **)ft_map_get("PATH", s->env));
	// printf("PATH: %s\n", paths[0]);
	// printf("PATH: %s\n", getenv("PATH"));
	// char **args = ft_get_args(s);
	// execve(, args, environ);
}