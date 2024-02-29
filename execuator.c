/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execuator.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: burkaya <burkaya@student.42istanbul.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/29 14:13:59 by burkaya           #+#    #+#             */
/*   Updated: 2024/02/29 18:07:42 by burkaya          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*ft_get_cmd_path(t_state *s)
{
	int		i;
	char	*path;
	char	**paths;
	paths = NULL;
	i = 0;

	while (s->env[i])
	{
		if (ft_strncmp(s->env[i], "PATH=", 5) == 0)
		{
			paths = ft_split(s->env[i] + 5, ':', s);
			break ;
		}
		i++;
	}
	i = 0;
	while (paths[i])
	{
		
		path = ft_strjoin(paths[i], "/", s);
		path = ft_strjoin(path, s->tokens->value, s);
		if (access(path, F_OK) == 0)
			return (path);
		i++;
	}
	return (NULL);
}

int	ft_find_token_amount(t_token *tokens)
{
	int	amount;

	amount = 0;
	while (tokens && tokens->type != T_CMD && tokens->type != T_PIPE)
	{
		amount++;
		tokens = tokens->next;
	}
	return (amount);
}

char	**ft_get_args(t_state *s, t_token **tokens, t_exec *exec)
{
	char	**args;
	int		i;
	int	amount;

	amount = ft_find_token_amount(*tokens);
	i = 1;
	args = (char **)malloc(sizeof(char *) * (amount + 2));
    args[0] = ft_strdup(exec->cmd_path, s);
	while (i < amount + 1)
	{
		args[i] = ft_strdup((*tokens)->value, s);
		(*tokens) = (*tokens)->next;
		i++;
	}
	return (args);
}

void    ft_get_cmd(t_state *s, t_exec *exec)
{
	while (s->tokens)
	{
		if (s->tokens->type == T_CMD)
		{
			exec->cmd_path = ft_get_cmd_path(s);
			s->tokens = s->tokens->next;
		}
		else
			exec->cmd_args = ft_get_args(s, &s->tokens, exec);
	}
}

void	ft_execuator(t_state *s)
{
	t_exec  *exec;

	exec = malloc(sizeof(t_exec));
	ft_get_cmd(s, exec);
    // printf("cmd_path: %s\n", exec->cmd_path);
    // int i = 0;
    // while (exec->cmd_args[i])
    // {
    //     printf("cmd_args: %s\n", exec->cmd_args[i]);
    //     i++;
    // }
	if (fork() == 0)
	    execve(exec->cmd_path, exec->cmd_args, s->env);
	else
	{
	    waitpid(-1, &s->status, 0);
	}    
}

