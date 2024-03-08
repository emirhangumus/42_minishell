/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   garbage.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: egumus <egumus@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/18 19:11:50 by egumus            #+#    #+#             */
/*   Updated: 2024/03/06 21:10:11 by egumus           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_add_garbage(t_state *s, void *ptr)
{
	t_garbage	*new;
	t_garbage	*tmp;

	new = (t_garbage *)malloc(sizeof(t_garbage));
	if (!new)
		return ;
	new->ptr = ptr;
	new->next = NULL;
	if (!s->garbage)
		s->garbage = new;
	else
	{
		tmp = s->garbage;
		while (tmp->next)
			tmp = tmp->next;
		tmp->next = new;
	}
}

void	ft_addarr_garbage(t_state *s, void **ptr)
{
	size_t	i;

	i = 0;
	while (ptr[i] != NULL)
	{
		ft_add_garbage(s, ptr[i]);
		i++;
	}
	ft_add_garbage(s, ptr);
}

void	ft_free_garbage(t_state *s)
{
	t_garbage	*tmp;

	while (s->garbage)
	{
		tmp = s->garbage;
		s->garbage = s->garbage->next;
		if (tmp->ptr != NULL)
		{
			free(tmp->ptr);
			tmp->ptr = NULL;
			free(tmp);
		}
	}
}
