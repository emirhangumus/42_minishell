/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   garbage.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: egumus <egumus@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/15 03:26:30 by egumus            #+#    #+#             */
/*   Updated: 2024/02/15 03:37:21 by egumus           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_garbage	*ft_garbage_new(void *content)
{
	t_garbage	*node;

	node = (t_garbage *)malloc(sizeof(t_garbage));
	if (!node)
		return (NULL);
	node->adress = content;
	node->next = NULL;
	return (node);
}

void	ft_add_garbage(t_garbage **garbage, void *adress)
{
	t_garbage	*current;
	t_garbage	*new;

	new = ft_garbage_new(adress);
	if (!new)
		return ;
	if (!garbage || !(*garbage))
	{
		*garbage = new;
		return ;
	}
	current = *garbage;
	while (current->next != NULL)
		current = current->next;
	current->next = new;
}

void	ft_free_garbage(t_garbage **garbage)
{
	t_garbage	*current;
	t_garbage	*next;

	if (!garbage || !(*garbage))
		return ;
	current = *garbage;
	while (current)
	{
		next = current->next;
		free(current->adress);
		free(current);
		current = next;
	}
	*garbage = NULL;
}