/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execuator.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: burkaya <burkaya@student.42istanbul.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/29 14:13:59 by burkaya           #+#    #+#             */
/*   Updated: 2024/03/01 12:18:33 by burkaya          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**ft_get_args(t_state *s, t_token *tokens)
{
	char	**args;
	int		i;
	int		arg_amount;

	i = 1;
	arg_amount = ft_find_arg_amount(tokens);
	args = malloc(sizeof(char *) * (arg_amount + 2));
	args[0] = tokens->value;
	tokens = tokens->next;
	if (args == NULL)
		return (NULL);
	args[arg_amount + 1] = NULL;
	while (i <= arg_amount)
	{
		args[i] = ft_strdup(tokens->value, s);
		tokens = tokens->next;
		i++;
	}
	return (args);
}

int	ft_init_execs(t_state *s, t_exec **exec)
{
	int		j;
	t_token	*tmp;

	j = 0;
	tmp = s->tokens;
	while (tmp)
	{
		if (tmp->type == T_CMD)
		{
			exec[j] = malloc(sizeof(t_exec));
			if (exec[j] == NULL)
				return (1);
			if (ft_is_builtin(tmp->value))
				exec[j]->type = CMD_BUILTIN;
			else
				exec[j]->type = CMD_PATH;
			exec[j]->cmd_path = ft_get_cmd_path(tmp, s);
			if (exec[j]->cmd_path == NULL && !(exec[j]->type == CMD_BUILTIN))
				return (ERR_CMD_NOT_FOUND);
			exec[j]->cmd_args = ft_get_args(s, tmp);
			j++;
		}
		tmp = tmp->next;
	}
	return (0);
}

void	ft_for_child(t_state *s, int i, int pipefd[2], t_exec **exec)
{
	if (i != 0)
	{
		dup2(s->fd, 0);
		close(s->fd);
	}
	if (exec[i + 1])
	{
		close(pipefd[0]);
		dup2(pipefd[1], 1);
	}
	if (exec[i]->type == CMD_BUILTIN)
		exit(ft_execute_builtin(exec[i], s, pipefd));
	execve(exec[i]->cmd_path, exec[i]->cmd_args, s->env);
	exit(0);
}

void	ft_run_pipes(t_state *s, t_exec **exec)
{
	int	i;
	int	pipefd[2];

	i = -1;
	while (exec[++i])
	{
		pipe(pipefd);
		if (exec[i]->type == CMD_BUILTIN && !exec[i + 1])
		{
			s->status = ft_execute_builtin(exec[i], s, pipefd);
			continue ;
		}
		s->pid = fork();
		if (s->pid == 0)
			ft_for_child(s, i, pipefd, exec);
		else
		{
			waitpid(s->pid, &s->status, 0);
			if (i != 0)
				close(s->fd);
			close(pipefd[1]);
			s->fd = pipefd[0];
		}
	}
}

int	ft_execuator(t_state *s)
{
	t_exec	**exec;
	int		cmd_amount;
	int		i;
	int		err_no;

	i = 0;
	cmd_amount = ft_amount_cmd(s->tokens);
	exec = malloc(sizeof(t_exec *) * (cmd_amount + 1));
	exec[cmd_amount] = NULL;
	err_no = ft_init_execs(s, exec);
	if (err_no)
		return (err_no);
	ft_run_pipes(s, exec);
	return (0);
}
