/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: burkaya <burkaya@student.42istanbul.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/19 17:51:51 by egumus            #+#    #+#             */
/*   Updated: 2024/03/30 00:06:06 by burkaya          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void ft_remove_char_by_index(char **str, int index, t_state *s)
{
	char *new;
	int i;
	int j;

	i = 0;
	j = 0;
	if (!str || !*str)
		return;
	if (index < 0 || index > (int)ft_strlen(*str))
		return;
	new = (char *)malloc(ft_strlen(*str));
	if (!new)
		return;
	while ((*str)[i])
	{
		if (i != index)
		{
			new[j] = (*str)[i];
			j++;
		}
		i++;
	}
	new[j] = '\0';
	if (s)
		ft_add_garbage(s, new);
	*str = new;
}

char *ft_joinstr_index(char *s1, char *s2, int start_index, t_state *s)
{
	char *new;
	int i;
	int j;
	int k;

	i = 0;
	j = 0;
	new = (char *)malloc(ft_strlen(s1) + ft_strlen(s2) + 1);
	if (!new)
		return (NULL);
	while (s1[i] && i < start_index)
	{
		new[i] = s1[i];
		i++;
	}
	k = i;
	while (s2[j])
	{
		new[i] = s2[j];
		i++;
		j++;
	}
	while (s1[k])
	{
		new[i] = s1[k];
		i++;
		k++;
	}
	new[i] = '\0';
	if (s)
		ft_add_garbage(s, new);
	return (new);
}

int ft_isalnum(char c)
{
	if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_')
		return (1);
	return (SUCCESS);
}

void ft_remove_key(char **str, int start_index, int end_index, t_state *s)
{
	while (end_index != start_index)
	{
		ft_remove_char_by_index(str, start_index, s);
		end_index--;
	}
}

void ft_calc_dollars(char *str, t_lexer *l, t_state *s)
{
	int i;
	int count;
	int quote;

	i = 0;
	count = 0;
	quote = QUOTE_NONE;
	while (str[i])
	{
		if (str[i] == '$')
			count++;
		i++;
	}
	l->meta = malloc(sizeof(t_lexer_meta));
	if (!l->meta)
		return;
	ft_add_garbage(s, l->meta);
	l->meta->dollars = malloc(sizeof(int) * count);
	if (!l->meta->dollars)
		return;
	ft_add_garbage(s, l->meta->dollars);
	i = 0;
	count = 0;
	while (str[i])
	{
		if (quote == QUOTE_NONE && (str[i] == '\'' || str[i] == '\"'))
			quote = str[i];
		else if (quote == str[i])
			quote = QUOTE_NONE;
		if (str[i] == '$')
		{
			if (quote == QUOTE_NONE || quote == QUOTE_TWO)
			{
				if (ft_isalnum(str[i + 1]) || str[i + 1] == '?')
					l->meta->dollars[count] = 1;
				else
					l->meta->dollars[count] = 0;
			}
			else
				l->meta->dollars[count] = 0;
			count++;
		}
		i++;
	}
}

int ft_merge_args(char **str, t_state *s, t_lexer *l)
{
	int quote;
	int i;
	int rm1;
	int rm2;
	int j;
	char *key;
	char *value;
	int dollar_counter;
	int len;

	i = 0;
	j = 0;
	dollar_counter = 0;
	rm1 = -1;
	rm2 = -1;
	quote = QUOTE_NONE;
	while ((*str)[i])
	{
		j = i;
		if ((*str)[i] == '$')
		{
			if (l->meta->dollars[dollar_counter] == 1)
			{
				j++;
				if ((*str)[j] == '?')
				{
					key = ft_strdup("?", s);
					value = ft_itoa(s->status, s);
					ft_remove_key(str, i, j + 1, s);
				}
				else
				{
					while ((*str)[j] && ft_isalnum((*str)[j]))
						j++;
					key = ft_substr(*str, i + 1, j - i - 1, s);
					value = ft_get_env(s->env, key);
					ft_remove_key(str, i, j, s);
					if (!value)
						value = ft_strdup("", s);
				}
				len = ft_strlen(value);
				*str = ft_joinstr_index(*str, value, i, s);
				if (len > 0)
					i += len - 1;
			}
			dollar_counter++;
		}
		if (!(*str)[i])
			break;
		if (quote == QUOTE_NONE && ((*str)[i] == '\'' || (*str)[i] == '\"'))
		{
			rm1 = i;
			quote = (*str)[i];
		}
		else if (quote == (*str)[i])
		{
			rm2 = i;
			quote = QUOTE_NONE;
		}
		if (rm1 > -1 && rm2 > -1)
		{
			ft_remove_char_by_index(str, rm1, s);
			ft_remove_char_by_index(str, rm2 - 1, s);
			rm1 = -1;
			rm2 = -1;
			i -= 2;
		}
		i++;
	}
	if (quote != QUOTE_NONE)
		return (write(2, "minishell: syntax error\n", 25), 1);
	return (SUCCESS);
}

char **ft_split_merge(char **split, t_state *s)
{
	int i;
	int j;
	int k;
	char **new_split;
	char **temp;
	int count;

	i = 0;
	j = 0;
	k = 0;
	count = 0;
	while (split[i])
	{
		temp = ft_quote_split(split[i], " ", s);
		count += ft_arr_len(temp);
		i++;
	}
	new_split = (char **)malloc(sizeof(char *) * (count + 1));
	if (!new_split)
		return (NULL);
	i = 0;
	j = 0;
	while (split[i])
	{
		temp = ft_quote_split(split[i], " ", s);
		k = 0;
		while (temp[k])
		{
			new_split[j] = ft_strdup(temp[k], s);
			j++;
			k++;
		}
		i++;
	}
	new_split[j] = NULL;
	if (s)
		ft_add_garbage(s, new_split);
	return (new_split);
}

int ft_is_redirect(char *str, char *original)
{
	int	quote;
	int	i;

	i = 0;
	quote = QUOTE_NONE;
	if (original)
	{
		while (original[i])
		{
			if (quote == QUOTE_NONE && (original[i] == '\'' || original[i] == '\"'))
				quote = original[i];
			else if (quote == original[i])
				quote = QUOTE_NONE;
			
			if (quote == QUOTE_NONE && ft_strncmp(original + i, "<<", 2) == 0)
				return (2);
			if (quote == QUOTE_NONE && ft_strncmp(original + i, ">>", 2) == 0)
				return (2);
			if (quote == QUOTE_NONE && ft_strncmp(original + i, ">", 1) == 0)
				return (1);
			if (quote == QUOTE_NONE && ft_strncmp(original + i, "<", 1) == 0)
				return (1);
			i++;
		}
		return (SUCCESS);
	}
	else
	{
		if (ft_strncmp(str, "<<", 2) == 0)
			return (2);
		if (ft_strncmp(str, ">>", 2) == 0)
			return (2);
		if (ft_strncmp(str, ">", 1) == 0)
			return (1);
		if (ft_strncmp(str, "<", 1) == 0)
			return (1);
		return (SUCCESS);
	}
}

char **ft_split_specials(char *str, t_state *s)
{
	char **sp;
	int i;
	int j;
	int quote;
	int is_redirect;

	i = 0;
	j = 0;
	is_redirect = 0;
	quote = QUOTE_NONE;
	sp = ft_quote_split(str, " ", s);
	while (sp[i])
	{
		j = 0;
		while (sp[i][j])
		{
			if (quote == QUOTE_NONE && (sp[i][j] == '\'' || sp[i][j] == '\"'))
				quote = sp[i][j];
			else if (quote == sp[i][j])
				quote = QUOTE_NONE;
			is_redirect = ft_is_redirect(sp[i] + j, NULL);
			if (quote == QUOTE_NONE && is_redirect > 0)
			{
				ft_extend_str_by_index(&(sp[i]), j, ' ', s);
				ft_extend_str_by_index(&(sp[i]), j + is_redirect + 1, ' ', s);
				j = j + is_redirect;
			}
			j++;
		}
		i++;
	}
	return (ft_split_merge(sp, s));
}

int ft_lexer_create(t_lexer *l, t_state *s)
{
	char	**split;
	int		i;
	int		is_redirect;
	char	*original;

	split = ft_split_specials(l->str, s);
	if (!split)
		return (ERR_MALLOC);
	i = 0;
	is_redirect = 0;
	while (split[i])
	{
		original = ft_strdup(split[i], s);
		ft_calc_dollars(split[i], l, s);
		if (ft_merge_args(&(split[i]), s, l))
			return (ERR_UNEXPECTED_TOKEN);
		is_redirect = ft_is_redirect(split[i], original);
		if (is_redirect > 0)
		{
			if (is_redirect == 2 && split[i + 1] && split[i][0] == '<')
				ft_add_token(s, split[i], T_LAPPEND, l->i);
			else if (is_redirect == 2 && split[i + 1] && split[i][0] == '>')
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
		i++;
	}
	return (SUCCESS);
}

static int ft_remove_tokens_command(t_token **token, int (*f)(void *))
{
	t_token *tmp;
	t_token *prev;
	int i;

	i = 0;
	tmp = *token;
	prev = NULL;
	while (tmp)
	{
		if (f(tmp->value))
		{
			if (prev)
				prev->next = tmp->next;
			else
				*token = tmp->next;
			free(tmp->value);
			free(tmp);
			tmp = prev;
		}
		if (!tmp)
		{
			if (i == 0 && *token)
				(*token)->type = T_CMD;
			tmp = *token;
			continue ;
		}
		prev = tmp;
		tmp = tmp->next;
		i++;
	}
	return (SUCCESS);
}

int ft_remove_tokens(t_token ***token, int (*f)(void *))
{
	int i;
	t_token **tmp;
	int err;

	i = 0;
	err = 0;
	tmp = *token;
	while (tmp[i])
	{
		err = ft_remove_tokens_command(&tmp[i], f);
		if (err)
			return (err);
		i++;
	}
	return (SUCCESS);
}

void ft_l_free_meta(t_lexer *l)
{
	if (l->meta->dollars)
		free(l->meta->dollars);
	free(l->meta);
}

void ft_redirect_arrange(t_token **tokens)
{
	t_token	**tmp1;
	t_token *tmp2;
	int		i;

	i = 0;
	tmp1 = tokens;
	while (tmp1[i])
	{
		tmp2 = tmp1[i];
		if (tmp2->prev == NULL && ft_is_redirect(tmp2->value, NULL) && tmp2->next && tmp2->next->next)
			tmp2->next->next->type = T_CMD;
		i++;	
	}
}

int ft_lexer(t_state *s)
{
	t_lexer *l;
	int pipes;
	char **sp;
	int err;

	err = 0;
	l = (t_lexer *)malloc(sizeof(t_lexer));
	if (!l)
		return (SUCCESS);
	pipes = ft_count_pipes(s->cmd);
	l->i = 0;
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
	err = ft_remove_tokens(&s->tokens, (int (*)(void *))ft_is_empty);
	free(l);
	if (err)
		return (err);
	ft_redirect_arrange(s->tokens);
	return (SUCCESS);
}
