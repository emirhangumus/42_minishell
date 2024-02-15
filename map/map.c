/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: egumus <egumus@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/15 00:06:00 by egumus            #+#    #+#             */
/*   Updated: 2024/02/15 04:02:52 by egumus           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	ft_map_get_index(char *key, t_map **map)
{
	int	i;

	i = -1;
	while (map[++i])
		if (ft_strcmp(map[i]->key, key) == 0)
			return (i);
	return (-1);
}

int	ft_map_insert(char *key, void *value, t_map **map)
{
	int	i;

	i = 0;
	while (map[i])
	{
		if (ft_strcmp(map[i]->key, key) == 0)
			return (1);
		i++;
	}
	map[i] = malloc(sizeof(t_map));
	if (!map[i])
		return (1);
	map[i]->key = key;
	map[i]->value = value;
	map[i + 1] = NULL;
	return (0);
}

void	ft_map_remove(char *key, t_map **map)
{
	int	i;

	i = ft_map_get_index(key, map);
	if (i == -1)
		return ;
	free(map[i]->key);
	free(map[i]->value);
	free(map[i]);
	while (map[i])
	{
		map[i] = map[i + 1];
		i++;
	}
	map[i] = NULL;
}

void	ft_map_free(t_map **map)
{
	int	i;

	i = -1;
	while (map[++i])
	{
		free(map[i]->key);
		free(map[i]->value);
		free(map[i]);
	}
	free(map);
}

void	ft_map_print(t_map **map)
{
	int	i;
	int	j;

	i = -1;
	j = 0;
	while (map[++i])
	{
		if (ft_strcmp(map[i]->key, "PATH") == 0)
		{
			while (j < ft_tab_len((char **)map[i]->value))
			{
				if (j == 0)
					printf("PATH=");
				printf("%s", ((char **)map[i]->value)[j]);
				if (j == ft_tab_len((char **)map[i]->value) - 1)
					printf("\n");
				else
					printf(":");
				j++;
			}
			j = 0;
		}
		else
		{
			printf("%s=%s\n", map[i]->key, (char *)map[i]->value);
		}
	}
}

void *ft_map_get(char *key, t_map **map)
{
	int	i;

	i = -1;
	while (map[++i])
		if (ft_strcmp(map[i]->key, key) == 0)
			return (map[i]->value);
	return (NULL);
}
