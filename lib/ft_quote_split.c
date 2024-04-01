/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_quote_split.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: egumus <egumus@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/20 21:23:06 by egumus            #+#    #+#             */
/*   Updated: 2024/04/01 02:20:49 by egumus           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	ft_count_words_with_quotes(char *s, char *c)
{
	int		count;
	int		seen_qt;
	int		toggle;

	count = 0;
	toggle = 0;
	seen_qt = 0;
	while (*s)
	{
		if (seen_qt == QUOTE_NONE && (*s == '\'' || *s == '\"'))
			seen_qt = (int)*s;
		else if (seen_qt == *s)
			seen_qt = QUOTE_NONE;
		if (seen_qt == QUOTE_NONE && ft_strncmp(s, c, ft_strlen(c)) == 0)
		{
			count++;
			toggle = 0;
		}
		else
			toggle = 1;
		s++;
	}
	if (toggle == 1)
		count++;
	return (count);
}

static	void	ft_set_quote_type(t_quote_split *qs)
{
	if (qs->seen_quote_type == QUOTE_NONE
		&& (qs->s[qs->i] == '\'' || qs->s[qs->i] == '\"'))
		qs->seen_quote_type = (int)qs->s[qs->i];
	else if (qs->seen_quote_type == (int)qs->s[qs->i])
		qs->seen_quote_type = QUOTE_NONE;
	qs->i++;
}

static int	ft_quote_split_loop(char **result, t_quote_split *qs, t_state *s)
{
	if (ft_strlen(qs->c) == ft_strlen(qs->s) \
		&& ft_strncmp(qs->s, qs->c, ft_strlen(qs->c)) == 0)
	{
		result[qs->j] = ft_strdup("", s);
		if (!result[qs->j])
			return (ft_free_prevs(result, qs->j), 1);
		return (2);
	}
	result[qs->j++] = ft_substr(qs->s, qs->k, qs->i - qs->k, s);
	if (!result[qs->j - 1])
		return (ft_free_prevs(result, qs->j - 1), 1);
	if (qs->s[qs->i])
		qs->i += ft_strlen(qs->c) - 1;
	return (0);
}

static int	ft_quote_split_helper(char **result, \
	t_quote_split *qs, t_state *state)
{
	int	ret;

	while (qs->s[qs->i])
	{
		qs->k = qs->i;
		while (qs->s[qs->i] && (qs->seen_quote_type != QUOTE_NONE
				|| ft_strncmp(&(qs->s[qs->i]), qs->c, ft_strlen(qs->c)) != 0))
			ft_set_quote_type(qs);
		if (qs->i > qs->k)
		{
			ret = ft_quote_split_loop(result, qs, state);
			if (ret == 1)
				return (1);
			if (ret == 2)
				break ;
		}
		if (!qs->s[qs->i])
			break ;
		qs->i++;
	}
	return (0);
}

char	**ft_quote_split(char *s, char *c, t_state *state)
{
	char			**result;
	t_quote_split	*qs;

	result = (char **)malloc(sizeof(char *) * \
		(ft_count_words_with_quotes(s, c) + 1));
	if (!result)
		return (NULL);
	qs = (t_quote_split *)malloc(sizeof(t_quote_split));
	if (!qs)
		return (ft_free_prevs(result, 0));
	qs->seen_quote_type = 0;
	qs->i = 0;
	qs->j = 0;
	qs->k = 0;
	qs->s = s;
	qs->c = c;
	if (ft_quote_split_helper(result, qs, state))
		return (free(qs), NULL);
	result[qs->j] = NULL;
	free(qs);
	ft_add_garbage(state, result);
	return (result);
}
