/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: egumus <egumus@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/14 22:59:11 by egumus            #+#    #+#             */
/*   Updated: 2024/02/15 03:53:27 by egumus           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <string.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <readline/history.h>
# include <signal.h>
# include <stdarg.h>
# include "./libft/libft.h"

# define T_COMMAND 1
# define T_OPERATOR 2
# define T_FLAG 3
# define T_PARAM 4

typedef struct s_map
{
	char	*key;
	void	*value;
} t_map;

typedef struct s_token
{
	int				type;
	char			*value;
	struct s_token	*next;
} t_token;

typedef struct s_garbage
{
	void				*adress;
	struct s_garbage	*next;
} t_garbage;

typedef struct s_state
{
	t_map		**env;
	t_token		*tokens;
	t_garbage	*garbage;
} t_state;

/* UTILS */
int			ft_tab_len(char **arr);
void		ft_free_tab(char **arr);
int			ft_strcmp(const char *s1, const char *s2);
char		**ft_msplit(char const *s, char c, t_state *state);

/* MAP */
int		ft_map_get_index(char *key, t_map **map);
int		ft_map_insert(char *key, void *value, t_map **map);
void	ft_map_remove(char *key, t_map **map);
void	ft_map_free(t_map **map);
void	ft_map_print(t_map **map); // DEBUG
void	*ft_map_get(char *key, t_map **map);

/* PATH */
void	ft_free_env(t_state *s);
int		ft_make_env(t_state *s);

/* TOKENIZE */
void	ft_add_token(t_token **lst, t_token *new);
void	ft_parse(t_state *s, char *line);
void	ft_print_tokens(t_token *lst); // DEBUG

/* GARBAGE */
void	ft_add_garbage(t_garbage **garbage, void *adress);
void	ft_free_garbage(t_garbage **garbage);
char	*ft_mstrdup(const char *s, t_state *state);

#endif