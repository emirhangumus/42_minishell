#include "minishell.h"

int ft_toggle_quote(t_lexer *l, char c)
{
	if (c == '|')
		l->current_pipe_index++;
	if (c == '\'' || c == '\"')
	{
		if (l->seen_quote_type == QUOTE_NONE)
		{
			l->seen_quote_type = c;
			return (1);
		}
		else if (l->seen_quote_type == c)
		{
			l->seen_quote_type = QUOTE_NONE;
			return (1);
		}
	}
	return (0);
}

int	ft_l_remove_quotes(t_lexer *l, t_state *s)
{
	int i;

	i = 0;
	l->current_dollar_index = -1;
	l->current_pipe_index = -1;
	while (l->str[i])
	{
		if (l->str[i] == '$')
			l->current_dollar_index++;
		if (l->str[i] == '|')
			l->current_pipe_index++;
		if (l->seen_quote_type == QUOTE_NONE && (l->str[i] == '\'' || l->str[i] == '\"'))
		{
			l->seen_quote_type = l->str[i];
			l->q1i = i;
		}
		else if (l->seen_quote_type == l->str[i])
		{
			l->seen_quote_type = QUOTE_NONE;
			l->q2i = i;
		}
		if (l->str[i] == '$' && l->seen_quote_type == QUOTE_SINGLE)
			l->insert_env[l->current_dollar_index] = 0;
		if (l->str[i] == '|' && l->seen_quote_type != QUOTE_NONE)
			l->is_pipe_is_arg[l->current_pipe_index] = 1;
		if (l->q1i != -1 && l->q2i != -1)
		{
			ft_remove_char_by_index(&l->str, l->q1i, s);
			ft_remove_char_by_index(&l->str, l->q2i - 1, s);
			l->q1i = -1;
			l->q2i = -1;
			i = i - 2;
		}
		i++;
	}
	return (0);
}

int ft_isalnum(int c)
{
	if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'))
		return (1);
	return (0);
}

char *ft_l_extract_key(char *str, int index)
{
	int start = index;
	int end = index;

	// Find the end of the variable name
	while (str[end] != '\0' && ft_isalnum(str[end]))
	{
		end++;
	}

	// Allocate memory for the variable name (plus null terminator)
	char *key = malloc(end - start + 1);
	if (key == NULL)
	{
		return NULL; // Failed to allocate memory
	}

	// Copy the variable name into the new string
	for (int i = start; i < end; i++)
	{
		key[i - start] = str[i];
	}

	// Add the null terminator
	key[end - start] = '\0';

	return key;
}

int ft_l_key_len(char *str, int index)
{
	int start = index;
	int end = index;

	// Find the end of the variable name
	while (str[end] != '\0' && ft_isalnum(str[end]))
	{
		end++;
	}

	return end - start;
}

char *ft_get_env(char **env, char *key)
{
	int i = 0;
	while (env[i])
	{
		if (ft_strncmp(env[i], key, ft_strlen(key)) == 0 && env[i][ft_strlen(key)] == '=')
		{
			return env[i] + ft_strlen(key) + 1;
		}
		i++;
	}
	return NULL;
}

int	ft_l_update_meta(t_lexer *l, int added_sp_count)
{
	t_lmeta **new_meta;
	t_lmeta **tmp;
	t_lmeta *tmp_meta;
	int		i;
	int		flag;

	i = -1;
	flag = 0;
	tmp = l->meta;
	// printf("added_sp_count: %d\n", added_sp_count);
	new_meta = (t_lmeta **)malloc(sizeof(t_lmeta *) * (ft_arr_len((char **)tmp) + 1 + added_sp_count));
	if (!new_meta)
		return (1);
	while (tmp[++i])
	{
		tmp_meta = malloc(sizeof(t_lmeta));
		if (!tmp_meta)
			return (1);
		tmp_meta->forced_arg = tmp[i]->forced_arg;
		if (flag == 0)
		{
			tmp_meta->can_be_cmd = 1;
			flag = 1;
		}
		else
			tmp_meta->can_be_cmd = tmp[i]->can_be_cmd;
		new_meta[i] = tmp_meta;
	}

	if (added_sp_count == 0)
	{
		l->meta[l->i]->forced_arg = 1;
	}
	else
	{
		while (added_sp_count--)
		{
			tmp_meta = malloc(sizeof(t_lmeta));
			if (!tmp_meta)
				return (1);
			tmp_meta->forced_arg = 1;
			tmp_meta->can_be_cmd = 0;
			new_meta[i++] = tmp_meta;
		}
	}

	new_meta[i] = NULL;
	// print values of tmp_meta
	i = -1;
	// while (new_meta[++i])
	// {
	// 	printf("tmp_meta->forced_arg %d: %d & %d\n", i, tmp_meta->forced_arg, new_meta[i]->can_be_cmd);
	// }

	return (0);
}

int	ft_rescale_sp(t_lexer *l, int space_index)
{
	int		i;
	int		j;
	int		k;
	char	**new_sp;
	int		max_len;

	i = 0;
	j = 0;
	k = 0;
	max_len = ft_arr_len(l->sp) + 2;
	new_sp = (char **)malloc(sizeof(char *) * max_len);
	if (!new_sp)
		return (1);
	while (l->sp[i] && i < l->i)
	{
		new_sp[i] = ft_strdup(l->sp[i], NULL); 
		i++;
	}
	new_sp[i] = ft_substr(l->str, 0, space_index, NULL);
	j = i;
	i++;
	new_sp[i] = ft_substr(l->str, space_index + 1, ft_strlen(l->str) - space_index, NULL);
	k = i;
	while (l->sp[i] != NULL && i < max_len)
	{
		new_sp[i] = ft_strdup(l->sp[i], NULL);
		i++;
	}
	if (i == j + 1)
		i++;
	new_sp[i] = NULL;
	ft_free_tab(l->sp);
	l->sp = new_sp;
	// these have to forced argument typed
	i = -1;
	l->str = ft_strdup(l->sp[k], NULL);
	while (l->str[++i])
	{
		if (l->str[i] == ' ')
		{
			l->i++;
			ft_rescale_sp(l, i);
			l->i--;
			break;
		};
	}
	return (j);
}

int ft_l_env(t_lexer *l, t_state *s)
{
	int i;
	char *env;
	int len;
	char *tmp;

	i = 0;
	l->current_dollar_index = -1;
	// If there are no variables to replace, return
	if (l->insert_env == NULL)
		return (0);
	while (l->str[i])
	{
		if (l->str[i] == '$')
			l->current_dollar_index++;
		if (l->str[i] == '$' && l->insert_env[l->current_dollar_index] == 1)
		{
			env = ft_l_extract_key(l->str, i + 1);
			len = ft_l_key_len(l->str, i + 1);
			if (env == NULL)
				return (1); // Failed to extract key
			// use ft_joinstr_index to merge the strings
			tmp = ft_get_env(s->env, env);
			if (tmp)
			{
				// Replace the variable with its value
				ft_remove_char_by_index(&l->str, i, s);
				while (len > 0)
				{
					ft_remove_char_by_index(&l->str, i, s);
					len--;
				}
				l->str = ft_joinstr_index(l->str, tmp, i, s);
				i = i + ft_strlen(tmp);
				free(env);
				continue;
			}
			else
			{
				// Variable not found
				// remove the dollar sign and the variable name
				ft_remove_char_by_index(&l->str, i, s);
				while (len > 0)
				{
					ft_remove_char_by_index(&l->str, i, s);
					len--;
				}
				free(env);
				continue;
			}
			free(env);
		}
		i++;
	}
	i = -1;
	int	new_str_index = -1;
	int	old_len = -1;
	int	new_len = -1;
	while (l->sp[++old_len])
		continue;
	while (l->str[++i])
	{
		if (l->str[i] == ' ')
		{
			new_str_index = ft_rescale_sp(l, i);
			break;
		};
	}
	if (new_str_index != -1)
	{
		l->str = ft_strdup(l->sp[new_str_index], s);
	};
	while (l->sp[++new_len])
		continue;
	ft_l_update_meta(l, new_len - old_len);
	return (0);
}

int ft_l_init_loop_env(t_lexer *l)
{
	int i;
	int count;

	i = -1;
	count = 0;
	while (l->str[++i])
	{
		if (l->str[i] == '$')
			count++;
	}
	if (count > 0)
	{
		l->insert_env = (int *)malloc(sizeof(int) * count);
		if (!l->insert_env)
			return (1);
		i = 0;
		while (i < count)
		{
			l->insert_env[i] = 1;
			i++;
		}
	}
	else
		l->insert_env = NULL;
	return (0);
}

int ft_l_init_loop_pipe(t_lexer *l)
{
	int i;
	int count;

	i = -1;
	count = 0;
	while (l->str[++i])
	{
		if (l->str[i] == '|')
			count++;
	}
	if (count > 0)
	{
		l->is_pipe_is_arg = (int *)malloc(sizeof(int) * count);
		if (!l->is_pipe_is_arg)
			return (1);
		i = 0;
		while (i < count)
		{
			l->is_pipe_is_arg[i] = 0;
			i++;
		}
	}
	else
		l->is_pipe_is_arg = NULL;
	return (0);
}

int ft_l_init_loop(t_lexer *l)
{
	if (ft_l_init_loop_env(l))
		return (1);
	if (ft_l_init_loop_pipe(l))
		return (1);
	return (0);
}

int ft_take_it(t_lexer *l, t_state *s, int *i, int *j)
{
	if (l->i == 0 || l->is_pipe_added)
	{
		ft_create_token(&s->tokens,
						ft_substr(l->str, *j, *i - *j + 1, s), T_CMD);
	}
	else
	{
		ft_create_token(&s->tokens,
						ft_substr(l->str, *j, *i - *j + 1, s), T_ARG);
	}
	*j = *i + 1;
	l->take_it = 0;
	l->is_pipe_added = 0;
	return (0);
}

int ft_lexer_bychar_pipe(t_state *s, t_lexer *l, int *i, int *j)
{
	if (l->seen_quote_type == QUOTE_NONE && l->str[*i] == '|' && (l->is_pipe_is_arg == NULL || (l->is_pipe_is_arg != NULL && l->is_pipe_is_arg[l->current_pipe_index] == 0)))
	{
		if (l->toggle_pipe_flag != 1 && l->i == 0 && !l->is_pipe_added)
			ft_create_token(&s->tokens,
							ft_substr(l->str, *j, *i - *j, s), T_CMD);
		else if (l->toggle_pipe_flag != 1 && *i != 0 && !l->is_pipe_added)
			ft_create_token(&s->tokens,
							ft_trim_quotes(ft_substr(l->str, *j, *i - *j, s), s, 1), T_ARG);
		if (l->toggle_pipe_flag)
			l->toggle_pipe_flag = 0;
		if (l->meta[l->i] && l->meta[l->i]->forced_arg == 1)
		{
			ft_create_token(&s->tokens, ft_strdup("|", s), T_ARG);
			l->is_pipe_added = 0;
		}
		else if (l->meta[l->i] && l->meta[l->i]->forced_arg == 0)
		{
			ft_create_token(&s->tokens, ft_strdup("|", s), T_PIPE);
			l->is_pipe_added = 1;
		}
		*j = *i + 1;
	}
	return (0);
}

int ft_lexer_bychar_iterate(t_state *s, t_lexer *l, int *i, int *j)
{
	if (ft_toggle_quote(l, l->str[*i]))
		return (1);
	if (l->is_pipe_added)
	{
		*j = *i;
		while (l->str[*i] && l->str[*i] != '|' && l->str[*i] != ' ')
			(*i)++;
		ft_create_token(&s->tokens,
						ft_trim_quotes(ft_substr(l->str, *j, *i - *j, s), s, 1), T_CMD);
		l->is_pipe_added = 0;
		if (l->str[*i] == '|')
			l->toggle_pipe_flag = 1;
		return (1);
	}
	if (ft_lexer_bychar_pipe(s, l, i, j))
		return (1);
	if (!l->str[*i + 1] && l->seen_quote_type == QUOTE_NONE && !l->is_pipe_added)
		l->take_it = 1;
	if (l->take_it)
		ft_take_it(l, s, i, j);
	return (0);
}

int ft_l_tokinize(t_lexer *l, t_state *s)
{
	int i;
	int j;
	int ret;
	t_token *tmp;

	i = 0;
	j = 0;
	l->seen_quote_type = QUOTE_NONE;
	l->q1i = -1;
	l->q2i = -1;
	l->current_dollar_index = -1;
	l->current_pipe_index = -1;
	l->take_it = 0;
	tmp = s->tokens;
	while (l->str[i])
	{
		ret = ft_lexer_bychar_iterate(s, l, &i, &j);
		if (ret == 1)
			continue;
		if (ret == 2)
			break;
		i++;
	}
	// if (tmp == NULL)
	// 	ft_l_merge_tokens(s->tokens, s, T_ARG);
	// else
	// 	ft_l_merge_tokens(tmp->next, s, T_ARG);
	return (0);
}

int ft_lexer_loop(t_lexer *l, t_state *s)
{
	while (l->sp[l->i])
	{
		l->str = ft_current_str(l);
		if (ft_l_init_loop(l))
			return (1);
		if (ft_l_remove_quotes(l, s))
			return (2);
		if (ft_l_env(l, s))
			return (3);
		if (ft_l_tokinize(l, s))
			return (4);
		if (l->insert_env)
			free(l->insert_env);
		l->i++;
	}
	return (0);
}