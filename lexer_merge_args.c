/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_merge_args.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: egumus <egumus@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/31 23:46:32 by egumus            #+#    #+#             */
/*   Updated: 2024/04/01 03:50:03 by egumus           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	ft_merge_args_envs_normal(t_state *s, t_lexer *l, char **str)
{
	while ((*str)[l->m]
		&& (ft_is_valid_env_key_char((*str)[l->m])))
		l->m++;
	l->key = ft_substr(*str, l->k + 1, l->m - l->k - 1, s);
	l->value = ft_get_env(s->env, l->key);
	ft_remove_key(str, l->k, l->m, s);
	if (!l->value)
		l->value = ft_strdup("", s);
}

static int	ft_merge_args_env_decision(t_state *s, t_lexer *l, char **str)
{
	if ((*str)[l->m] == '?')
	{
		l->key = ft_strdup("?", s);
		l->value = ft_itoa(s->status, s);
		ft_remove_key(str, l->k, l->m + 1, s);
	}
	else
	{
		if (ft_isdigit((*str)[l->m]))
		{
			if ((*str)[l->m] == '0')
			{
				ft_remove_key(str, l->k, l->m + 1, s);
				l->value = s->exec_name;
			}
			else
				return (ft_remove_key(str, l->k, l->m + 1, s), 1);
		}
		else
			ft_merge_args_envs_normal(s, l, str);
	}
	return (0);
}

static int	ft_merge_args_env(t_state *s, t_lexer *l, char ***split, char **str)
{
	int		len;
	char	*temp;
	int		dollar_counter;

	dollar_counter = 0;
	if ((*str)[l->k] == '$')
	{
		if (l->meta->dollars[dollar_counter] == 1)
		{
			l->m++;
			if (ft_merge_args_env_decision(s, l, str))
				return (1);
			len = ft_strlen(l->value);
			temp = ft_strdup(*str, s);
			*str = ft_joinstr_index(*str, l->value, l->k, s);
			if (ft_strchr(ft_substr(*str, l->k, len, s), ' '))
				ft_add_envs_as_arg_or_cmd(l->current_pipe_index, s, split);
			if (len > 0)
				l->k += len - 1;
		}
		dollar_counter++;
	}
	return (0);
}

static void	ft_merge_args_remove_quotes(t_state *s, \
	t_lexer *l, char **str, int *quote)
{
	if (*quote == QUOTE_NONE && ((*str)[l->k] == '\'' || (*str)[l->k] == '\"'))
	{
		l->rm1 = l->k;
		*quote = (*str)[l->k];
	}
	else if (*quote == (*str)[l->k])
	{
		l->rm2 = l->k;
		*quote = QUOTE_NONE;
	}
	if (l->rm1 > -1 && l->rm2 > -1)
	{
		ft_remove_char_by_index(str, l->rm1, s);
		ft_remove_char_by_index(str, l->rm2 - 1, s);
		l->rm1 = -1;
		l->rm2 = -1;
		l->k -= 2;
	}
}

int	ft_merge_args(int index, t_state *s, t_lexer *l, char ***split)
{
	int		quote;
	char	**str;

	str = &((*split)[index]);
	l->current_pipe_index = index;
	l->k = 0;
	l->m = 0;
	l->rm1 = -1;
	l->rm2 = -1;
	l->key = NULL;
	l->value = NULL;
	quote = QUOTE_NONE;
	while ((*str)[l->k])
	{
		l->m = l->k;
		if (ft_merge_args_env(s, l, split, str))
			continue ;
		if (!(*str)[l->k])
			break ;
		ft_merge_args_remove_quotes(s, l, str, &quote);
		l->k++;
	}
	if (quote != QUOTE_NONE)
		return (ERR_UNEXPECTED_TOKEN);
	return (SUCCESS);
}
