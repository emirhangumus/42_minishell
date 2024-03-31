/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: egumus <egumus@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/31 22:23:57 by egumus            #+#    #+#             */
/*   Updated: 2024/03/31 23:30:38 by egumus           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_add_envs_as_arg_or_cmd(int index, t_state *s, char ***split)
{
	char	**str;
	int		i;
	int		j;
	char	**split2;

	i = 0;
	j = 0;
	str = &((*split)[index]);
	split2 = ft_quote_split(*str, " ", s);
	if (split2 != NULL)
		ft_arr_remove_by_index(split, index, s);
	while (split2[i])
	{
		ft_arr_add_by_index(split, split2[i], index, s);
		index++;
		i++;
	}
	return (SUCCESS);
}

void	ft_redirect_arrange(t_token **tokens)
{
	t_token	**tmp1;
	t_token	*tmp2;
	int		i;

	i = 0;
	tmp1 = tokens;
	while (tmp1[i])
	{
		tmp2 = tmp1[i];
		if (tmp2->prev == NULL && ft_is_redirect(tmp2->value, NULL)
			&& tmp2->next && tmp2->next->next)
			tmp2->next->next->type = T_CMD;
		i++;
	}
}

static void	ft_split_specials_helper(char **sp, int *i, t_state *s)
{
	int		j;
	int		is_redirect;
	int		quote;

	j = 0;
	quote = QUOTE_NONE;
	while (sp[*i][j])
	{
		if (quote == QUOTE_NONE && (sp[*i][j] == '\'' || sp[*i][j] == '\"'))
			quote = sp[*i][j];
		else if (quote == sp[*i][j])
			quote = QUOTE_NONE;
		is_redirect = ft_is_redirect(sp[*i] + j, NULL);
		if (quote == QUOTE_NONE && is_redirect > 0)
		{
			ft_extend_str_by_index(&(sp[*i]), j, ' ', s);
			ft_extend_str_by_index(&(sp[*i]), j + is_redirect + 1, ' ', s);
			j = j + is_redirect;
		}
		j++;
	}
}

char	**ft_split_specials(char *str, t_state *s)
{
	char	**sp;
	int		i;

	i = -1;
	sp = ft_quote_split(str, " ", s);
	while (sp[++i])
		ft_split_specials_helper(sp, &i, s);
	return (ft_split_merge(sp, s));
}
