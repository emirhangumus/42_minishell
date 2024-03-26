/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: burkaya <burkaya@student.42istanbul.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/01 14:29:10 by egumus            #+#    #+#             */
/*   Updated: 2024/03/26 15:19:39 by burkaya          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_print_tokens(t_token **token)
{
	t_token	**tmp;
	t_token	*next;
	int		i;

	if (!token || !*token)
		return ;
	tmp = token;
	i = 0;
	while (tmp[i])
	{
		next = tmp[i];
		while (next)
		{
			printf("Token %d: type:%d, %s$\n", i, next->type, next->value);
			next = next->next;
		}
		i++;
	}
}

void	ft_print_tab(char **tab)
{
	int	i;

	i = 0;
	while (tab[i])
	{
		printf("[%d]: %s\n", i, tab[i]);
		i++;
	}
}

void	ft_print_execs(t_exec **exec)
{
	int	i;
	int	j;

	i = 0;
	while (exec[i])
	{
		j = 0;
		printf("exec[%d]->cmd_path: %s\n", i, exec[i]->cmd_path);
		printf("exec[%d]->in: %d\n", i, exec[i]->in_type);
		printf("exec[%d]->out: %d\n", i, exec[i]->out_type);
		printf("exec[%d]->type: %d\n", i, exec[i]->type);
		printf("exec[%d]->in_file: %s\n", i, exec[i]->in_file);
		printf("exec[%d]->out_file: %s\n", i, exec[i]->out_file);
		while (exec[i]->cmd_args[j])
		{
			printf("exec[%d]->cmd_args[%d]: %s\n", i, j, exec[i]->cmd_args[j]);
			j++;
		}
		i++;
	}
}

void	ft_print_exec(t_exec *exec)
{
	int	i;

	i = 0;
	printf("exec->cmd_path: %s\n", exec->cmd_path);
	printf("exec->in: %d\n", exec->in_type);
	printf("exec->out: %d\n", exec->out_type);
	printf("exec->type: %d\n", exec->type);
	while (exec->cmd_args[i])
	{
		printf("exec->cmd_args[%d]: %s\n", i, exec->cmd_args[i]);
		i++;
	}
}
