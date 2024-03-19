/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: burkaya <burkaya@student.42istanbul.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/01 14:29:10 by egumus            #+#    #+#             */
/*   Updated: 2024/03/19 21:05:04 by burkaya          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_print_tokens(t_token **token)
{
	t_token	**tmp;
	t_token	*next;
	int		i;
	
	if (!token || !*token)
		return ;
	tmp = token;
	i = 0;
	while (tmp[i])
	{
		next = tmp[i];
		while (next)
		{
			printf("Token %d: type:%d, %s$\n", i, next->type, next->value);
			next = next->next;
		}
		i++;
	}
}

void ft_print_tab(char **tab)
{
	int i;

	i = 0;
	while (tab[i])
	{
		printf("[%d]: %s\n", i, tab[i]);
		i++;
	}
}

void	ft_print_execs(t_exec **exec)
{
	int i;
	int j;

	i = 0;
	while (exec[i])
	{
		j = 0;
		printf("exec[%d]->cmd_path: %s\n", i, exec[i]->cmd_path);
		while (exec[i]->cmd_args[j])
		{
			printf("exec[%d]->cmd_args[%d]: %s\n", i, j, exec[i]->cmd_args[j]);
			j++;
		}
		i++;
	}
}