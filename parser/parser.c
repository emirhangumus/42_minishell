/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: egumus <egumus@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/14 22:55:22 by egumus            #+#    #+#             */
/*   Updated: 2024/02/15 06:04:03 by egumus           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	ft_is_one_of_them(char *str, int len, ...)
{
	va_list	arg;
	int	i;

	i = 0;
	va_start(arg, len);
	while (i < len)
	{
		if (ft_strcmp(str, va_arg(arg, char *)) == 0)
			return (1);
		i++;
	}
	va_end(arg);
	return (0);
}

char *ft_check_operator_spaces(char *line , t_state *s)
{
	char *l;
	int	size;
	int	i;

	i = 0;
	l = ft_mstrdup(line, s);
	size = ft_strlen(l);
	char **h = ft_msplit_set(l, "<<", s);
	i = 0;
	while (i < ft_tab_len(h))
	{
		printf("h: %s\n", h[i]);
		i++;
	}
	return (l);
}

void	ft_parse(t_state *s, char *line)
{
	int	i;
	char **arr;
	char *tmp;

	i = 1;
	// line = ft_check_operator_spaces(line, s);
	arr = ft_msplit(line, ' ', s);
	t_token *token = malloc(sizeof(t_token));
	token->type = T_COMMAND;
	token->value = ft_strdup(arr[0]);
	token->next = NULL;
	ft_add_token(&s->tokens, token);
	while (arr[i])
	{
		// printf("i: %d\n", i);
		tmp = ft_mstrdup(arr[i], s);
		if (tmp[0] == '-' || (tmp[0] == '-' && tmp[1] == '-'))
		{
			// flag
			token = malloc(sizeof(t_token));
			token->type = T_FLAG;
			token->value = ft_strdup(tmp);
			token->next = NULL;
			ft_add_token(&s->tokens, token);
		}
		// else if (ft_is_one_of_them(tmp, 5, "<", ">", "<<", ">>", "|"))
		// {
		// 	printf("FUCK\n");
		// }
		else
		{
			token = malloc(sizeof(t_token));
			token->type = T_PARAM;
			token->value = ft_strdup(tmp);
			token->next = NULL;
			ft_add_token(&s->tokens, token);
		}
		i++;
	}
}