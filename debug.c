/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: egumus <egumus@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/01 14:29:10 by egumus            #+#    #+#             */
/*   Updated: 2024/03/19 17:42:44 by egumus           ###   ########.fr       */
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