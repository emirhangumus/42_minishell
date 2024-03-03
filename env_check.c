/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_check.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: egumus <egumus@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/03 17:51:35 by egumus            #+#    #+#             */
/*   Updated: 2024/03/03 19:08:03 by egumus           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_cmd_add_env(t_token *t, t_state *s, int i)
{
	int		j;
	int		env_len;
	char	*key;
	char	*value;

	j = i + 1;
	if (!t->value[j])
		return (-1);
	while (t->value[j] && t->value[j] != ' ')
		j++;
	key = ft_substr(t->value, i + 1, j - i - 1, s);
	value = ft_get_env(s->env, key);
	if (value != NULL)
	{
		env_len = 0;
		while (value[env_len])
			env_len++;
		t->value = ft_strjoin(ft_substr(t->value, 0, i, s), \
			ft_strjoin(value, ft_strdup(t->value + j, s), s), s);
	}
	else
	{
		t->value = ft_strjoin(ft_substr(t->value, 0, i, s), \
			ft_strdup(t->value + j, s), s);
	
	}
	return (j);
}

void	ft_env_check(t_state *s)
{
	int		i;
	int		h;
	t_token *t;

	t = s->tokens;
	while (t)
	{
		i = 0;
		while (t->value[i])
		{
			if (t->value[i] == '$')
			{
				h = ft_cmd_add_env(t, s, i);
				if (h != -1)
				{
					i = h;
					continue;
				}
			}
			i++;
		}
		t = t->next;
	}
}