/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_functions.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: egumus <egumus@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/15 01:31:02 by egumus            #+#    #+#             */
/*   Updated: 2024/02/15 01:37:08 by egumus           ###   ########.fr       */
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
