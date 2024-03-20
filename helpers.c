/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helpers.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: burkaya <burkaya@student.42istanbul.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/21 00:55:47 by egumus            #+#    #+#             */
/*   Updated: 2024/03/20 13:46:17 by burkaya          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*ft_get_env(char **env, char *key)
{
	int		i;
	size_t	len;

	i = 0;
	len = ft_strlen(key);
	while (env[i])
	{
		if (ft_strncmp(env[i], key, len) == 0 && env[i][len] == '=')
			return (env[i] + len + 1);
		i++;
	}
	return (NULL);
}

void	ft_extend_str_by_index(char **str, int index, char c, t_state *s)
{
	char	*new;
	int		i;
	int		j;

	i = 0;
	j = 0;
	new = (char *)malloc(ft_strlen(*str) + 2);
	if (!new)
		return ;
	while ((*str)[i])
	{
		if (i == index)
		{
			new[j] = c;
			j++;
		}
		new[j] = (*str)[i];
		i++;
		j++;
	}
	new[j] = '\0';
	if (s)
		ft_add_garbage(s, new);
	*str = new;
}

int	ft_count_pipes(char *cmd)
{
	int	quote;
	int	i;
	int	count;

	quote = QUOTE_NONE;
	i = 0;
	count = 0;
	while (cmd[i])
	{
		if (quote == QUOTE_NONE && (cmd[i] == '\'' || cmd[i] == '\"'))
			quote = cmd[i];
		else if (quote == cmd[i])
			quote = QUOTE_NONE;
		if (quote == QUOTE_NONE && cmd[i] == '|')
			count++;
		i++;
	}
	return (count);
}
