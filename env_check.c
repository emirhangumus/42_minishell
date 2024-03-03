/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_check.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: egumus <egumus@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/03 17:51:35 by egumus            #+#    #+#             */
/*   Updated: 2024/03/03 22:22:33 by egumus           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_isalphanum(char c)
{
	if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || \
		(c >= '0' && c <= '9') || c == '_')
		return (1);
	return (0);
}

int	ft_cmd_add_env(t_token *t, t_state *s, int i)
{
	int		j;
	int		env_len;
	char	*key;
	char	*value;

	j = i + 1;
	if (!t->value[j])
		return (-1);
	while (t->value[j] && t->value[j] != ' ' && t->value[j] != '$' && \
		ft_isalphanum(t->value[j]))
		j++;
	key = ft_substr(t->value, i + 1, j - i - 1, s);
	value = ft_get_env(s->env, key);
	printf("key: %s\n", key);
	printf("value: %s\n", value);
	if (value != NULL)
	{
		env_len = 0;
		while (value[env_len])
			env_len++;
		t->value = ft_strjoin(ft_substr(t->value, 0, i, s), \
			ft_strjoin(value, ft_strdup(t->value + j, s), s), s);
		return (j);
	}
	else
	{
		t->value = ft_strjoin(ft_substr(t->value, 0, i, s), \
			ft_strdup(t->value + j, s), s);
		printf("---------->value: %s\n", t->value);
		return (-1);
	}
}

void	ft_env_check(t_token *tmp, t_state *s)
{
	int		i;
	int		h;
	t_token	*t;

	t = tmp;
	i = 0;
	printf("-Z>value: %s\n", t->value);
	if (!t->value || !t->value[i] || t->value[i] == '\'')
		return;
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
			else
				i = 0;
		}
		i++;
	}
}