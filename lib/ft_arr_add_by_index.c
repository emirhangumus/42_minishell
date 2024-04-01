/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_arr_add_by_index.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: egumus <egumus@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/30 01:11:42 by burkaya           #+#    #+#             */
/*   Updated: 2024/04/01 03:03:23 by egumus           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	ft_arr_add_by_index_helper1(t_arr_add_by_index *aabi, \
	char *str, int index)
{
	if (aabi->i == index)
	{
		aabi->new_arr[aabi->j] = ft_strdup(str, NULL);
		if (aabi->new_arr[(aabi->j)++] == NULL)
			return (1);
	}
	return (0);
}

static int	ft_arr_add_by_index_helper2(t_arr_add_by_index *aabi, char ***arr)
{
	if ((*arr)[aabi->i] != NULL)
	{
		aabi->new_arr[aabi->j] = ft_strdup((*arr)[aabi->i], NULL);
		if (aabi->new_arr[(aabi->j)++] == NULL)
			return (1);
	}
	return (0);
}

void	ft_arr_add_by_index(char ***arr, char *str, int index, t_state *s)
{
	t_arr_add_by_index	*aabi;

	aabi = malloc(sizeof(t_arr_add_by_index));
	if (aabi == NULL)
		return ;
	aabi->len = ft_arr_len(*arr);
	aabi->new_arr = malloc(sizeof(char *) * (aabi->len + 2));
	if (aabi->new_arr == NULL)
		return ;
	aabi->i = -1;
	aabi->j = 0;
	while (++(aabi->i) <= aabi->len)
	{
		if (ft_arr_add_by_index_helper1(aabi, str, index))
			return ;
		if (ft_arr_add_by_index_helper2(aabi, arr))
			return ;
	}
	aabi->new_arr[aabi->j] = NULL;
	*arr = ft_arr_dup(aabi->new_arr, s);
	ft_free_tab(aabi->new_arr);
	free(aabi);
}
