/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_check.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: burkaya <burkaya@student.42istanbul.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/03 17:51:35 by egumus            #+#    #+#             */
/*   Updated: 2024/03/04 02:52:08 by burkaya          ###   ########.fr       */
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
<<<<<<< HEAD
		t->value = ft_strjoin(ft_substr(t->value, 0, i, s), \
			ft_strdup(t->value + j, s), s);
=======
		if (i != 0)
			t->value = ft_strjoin(ft_substr(t->value, 0, i, s), \
				ft_strdup(t->value + j, s), s);
		else
			t->value = ft_strdup("", s);
		if (!t->value || !t->value[0])
		{
			return (-2);
		}
>>>>>>> ff42252b050d2cc11ea5bf0b4f19e35707dbd733
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
<<<<<<< HEAD
	if (!t->value || !t->value[i] || t->value[i] == '\'')
=======
	if (!t || !t->value || !t->value[i] || t->value[i] == '\'')
>>>>>>> ff42252b050d2cc11ea5bf0b4f19e35707dbd733
		return;
	while (t->value && t->value[i] != 0)
	{
		if (t->value && t->value[i] == '$')
		{
			h = ft_cmd_add_env(t, s, i);
			if (h != -1)
			{
				i = h;
				continue;
			}
			else if (h == -2)
				return ;
			else
				i = 0;
		}
		i++;
	}
}