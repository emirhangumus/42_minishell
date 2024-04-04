/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: burkaya <burkaya@student.42istanbul.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/19 17:51:51 by egumus            #+#    #+#             */
/*   Updated: 2024/04/04 22:50:49 by burkaya          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	ft_lexer_create_token(t_state *s, t_lexer *l, char **split, int i)
{
	int		is_redirect;

	is_redirect = ft_is_redirect(split[i], l->original);
	if (is_redirect > 0)
	{
		if (is_redirect == 2 && split[i][0] == '<')
			ft_add_token(s, split[i], T_LAPPEND, l->i);
		else if (is_redirect == 2 && split[i][0] == '>')
			ft_add_token(s, split[i], T_RAPPEND, l->i);
		else if (is_redirect == 1 && split[i][0] == '<')
			ft_add_token(s, split[i], T_LREDIR, l->i);
		else if (is_redirect == 1 && split[i][0] == '>')
			ft_add_token(s, split[i], T_RREDIR, l->i);
	}
	else if (i == 0)
		ft_add_token(s, split[i], T_CMD, l->i);
	else
		ft_add_token(s, split[i], T_ARG, l->i);
	ft_get_last_token(s->tokens[l->i])->remove = l->remove_toggler;
}

int	ft_lexer_create(t_lexer *l, t_state *s)
{
	char	**split;
	int		i;

	split = ft_split_specials(l->str, s);
	if (!split)
		return (ERR_MALLOC);
	i = 0;
	while (split[i])
	{
		l->original = ft_strdup(split[i], s);
		ft_calc_dollars(split[i], l, s);
		if (ft_merge_args(i, s, l, &split))
			return (ERR_UNEXPECTED_TOKEN);
		ft_lexer_create_token(s, l, split, i);
		i++;
	}
	return (SUCCESS);
}

static int	ft_run_lexer(t_lexer *l, t_state *s, int pipes)
{
	int		err;
	char	**sp;

	l->i = 0;
	l->remove_toggler = 0;
	sp = ft_quote_split(s->cmd, "|", s);
	while (l->i <= pipes)
	{
		if (!sp[l->i])
			return (ERR_UNEXPECTED_TOKEN);
		l->str = ft_strdup(sp[l->i], s);
		err = ft_lexer_create(l, s);
		if (err)
			return (err);
		l->i++;
	}
	return (SUCCESS);
}

int	ft_lexer(t_state *s)
{
	t_lexer	*l;
	int		pipes;
	int		err;

	err = 0;
	l = (t_lexer *)malloc(sizeof(t_lexer));
	if (!l)
		return (SUCCESS);
	err = ft_check_invalid_pipes(s->cmd, 0);
	if (err)
		return (free(l), err);
	pipes = ft_count_pipes(s->cmd);
	err = ft_run_lexer(l, s, pipes);
	if (err)
		return (free(l), err);
	err = ft_remove_tokens(&s->tokens, (int (*)(void *))ft_is_empty);
	free(l);
	if (err)
		return (err);
	ft_redirect_arrange(s->tokens);
	err = ft_lexer_validate(s);
	if (err)
		return (err);
	return (SUCCESS);
}
