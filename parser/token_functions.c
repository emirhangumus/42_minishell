/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_functions.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: egumus <egumus@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/15 01:31:02 by egumus            #+#    #+#             */
/*   Updated: 2024/02/15 06:00:35 by egumus           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	ft_add_token(t_token **lst, t_token *new)
{
	t_token	*current;

	if (!lst || !(*lst))
	{
		*lst = new;
		return ;
	}
	current = *lst;
	while (current->next != NULL)
		current = current->next;
	current->next = new;
}

void	ft_free_tokens(t_token **lst)
{
	t_token	*current;
	t_token	*next;

	current = *lst;
	while (current != NULL)
	{
		next = current->next;
		free(current->value);
		free(current);
		current = next;
	}
	*lst = NULL;
}

int	ft_token_size(t_token *head, int type)
{
	int		i;
	t_token *tmp;
	
	if (!head)
			return (0);
	tmp = head;
	i = 0;
	while (tmp)
	{
		if (type > 0)
		{
			if (tmp->type == type)
				i++;
		}
		else
			i++;
		tmp = tmp->next;
	}
	return (i);
}

void	ft_print_tokens(t_token *lst)
{
	t_token	*current;

	current = lst;
	while (current != NULL)
	{
		printf("type: %d, value: %s\n", current->type, current->value);
		current = current->next;
	}
}
