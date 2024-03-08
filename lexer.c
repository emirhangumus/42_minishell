#include "minishell.h"

char	*ft_current_str(t_lexer *l)
{
	return (l->sp[l->i]);
}

int	ft_lexer_meta_init(t_lexer *l)
{
	int	i;

	i = ft_arr_len(l->sp);
	l->meta = (t_lmeta **)malloc(sizeof(t_lmeta *) * (i + 1));
	if (!l->meta)
		return (1);
	i = -1;
	while (l->sp[++i])
	{
		l->meta[i] = (t_lmeta *)malloc(sizeof(t_lmeta));
		if (!l->meta[i])
			return (1);
		l->meta[i]->forced_arg = 0;
		l->meta[i]->can_be_cmd = 0;
	}
	l->meta[i] = NULL;
	return (0);
}

int	ft_lexer_init(t_lexer *l, t_state *s)
{
	l->i = 0;
	l->seen_quote_type = QUOTE_NONE;
	l->is_pipe_added = 0;
	l->take_it = 0;
	l->sp = ft_quote_split(s->cmd, ' ', s);
	l->result = NULL;
	l->q1i = -1;
	l->q2i = -1;
	l->toggle_pipe_flag = 0;
	l->current_dollar_index = 0;
	l->current_pipe_index = 0;
	if (!l->sp)
		return (1);
	l->str = ft_current_str(l);
	if (ft_lexer_meta_init(l))
		return (1);
	return (0);
}

int	ft_lexer(t_state *s)
{
	t_lexer	*l;

	l = (t_lexer *)malloc(sizeof(t_lexer));
	if (!l)
		return (1);
	if (ft_lexer_init(l, s))
		return (free(l), 1);
	if (ft_lexer_loop(l, s))
		return (free(l), 1);
	free(l);
	return (ft_remove_tokens(&s->tokens, (int (*)(void *))ft_is_empty), 0);
}
