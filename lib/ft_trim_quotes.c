/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_trim_quotes.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: egumus <egumus@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/28 17:46:24 by egumus            #+#    #+#             */
/*   Updated: 2024/03/03 20:39:31 by egumus           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*ft_trim_quotes(char const *str, t_state *s, int n)
{
	char	*new_str;
	size_t	i;
	size_t	len;

	if (!str)
		return (NULL);
	i = 0;
	len = ft_strlen(str);
	while (n > 0 && str[i] && ((str[i] == '\'' && str[len - i - 1] == '\'')
			|| (str[i] == '\"' && str[len - i - 1] == '\"')))
	{
		i++;
		n--;
	}
	new_str = ft_substr(str, i, len - i * 2, s);
	return (new_str);
}
