/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_check.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: burkaya <burkaya@student.42istanbul.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/03 17:51:35 by egumus            #+#    #+#             */
/*   Updated: 2024/03/26 15:17:20 by burkaya          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_is_valid_env_name(char c)
{
	if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || \
		(c >= '0' && c <= '9') || c == '_')
		return (1);
	return (SUCCESS);
}

int	ft_cmd_add_env(t_token *t, t_state *s, int i)
{
	int		j;
	int		env_len;
	char	*key;
	char	*value;

	j = i + 1;
	if (!t->value || !t->value[j])
		return (ENV_ERROR);
	if (t->value[j] == '?')
	{
		t->value = ft_strjoin(ft_substr(t->value, 0, i, s), \
			ft_strjoin(ft_itoa(s->status, NULL), ft_strdup(t->value + j + 1, s), s), s);
		return (ENV_RECHECK);
	}
	while (t->value[j] && t->value[j] != ' ' && t->value[j] != '$' && t->value[j] != '?' && \
		ft_is_valid_env_name(t->value[j]))
		j++;
	key = ft_substr(t->value, i + 1, j - i - 1, s);
	value = ft_get_env(s->env, key);
	if (value != NULL)
	{
		env_len = 0;
		while (value[env_len])
			env_len++;
		if (env_len > 1)
			t->value = ft_strjoin(ft_substr(t->value, 0, i, s), \
				ft_strjoin(value, ft_strdup(t->value + j, s), s), s);
		else
		{
			t->value = ft_strjoin(ft_strjoin(ft_substr(t->value, 0, i, s), value, s), \
				ft_strdup(t->value + j, s), s);
			return (ENV_SINGLE_VALUE);
		}
		return (j);
	}
	else
	{
		t->value = ft_strjoin(ft_substr(t->value, 0, i, s), \
			ft_strdup(t->value + j, s), s);
		if (!t->value || !t->value[0])
			return (ENV_NO_MORE);
		return (ENV_RECHECK);
	}
}

void	ft_env_check(t_token *tmp, t_state *s)
{
	int		i;
	int		h;
	t_token	*t;

	t = tmp;
	i = 0;
	if (!t || !t->value || !t->value[i] || t->value[i] == '\'')
		return;
	while (t->value && t->value[i])
	{
		if (t->value && t->value[i] == '$')
		{
			h = ft_cmd_add_env(t, s, i);
			if (h > ENV_RECHECK)
			{
				i = h;
				continue;
			}
			else if (h == ENV_NO_MORE || h == ENV_ERROR)
				return ;
			else if (h == ENV_SINGLE_VALUE)
				i--;
			else
				i = 0;
		}
		i++;
	}
}
