/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execuator.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: burkaya <burkaya@student.42istanbul.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/29 14:13:59 by burkaya           #+#    #+#             */
/*   Updated: 2024/03/07 18:46:24 by burkaya          ###   ########.fr       */
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
	ft_add_garbage(s, args);
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
			ft_add_garbage(s, exec[j]);
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

void    ft_init_pipes(t_state* s, int cmd_amount, int **pipes, int **forks)
{
    int i;

    i = 0;
    *pipes = malloc(sizeof(int) * (cmd_amount * 2));
    *forks = malloc(sizeof(int) * (cmd_amount));
    while (i < cmd_amount)
    {
        if (pipe(*pipes + i * 2) < 0)
        {
            s->status = ERR_PIPE_INIT;
            return ;
        }
        i++;
    }
}

void    ft_lets_go(t_state *s, t_exec **exec, int cmd_amount, int **pipes, int **forks)
{
    int i;

    i = 0;
    while (exec[i])
    {
        (*forks)[i] = fork();
        if ((*forks)[i] == 0)
        {    
            if (i == 0)
            {
                dup2((*pipes)[i * 2 + 1], 1);
                close_pipes_all(*pipes, cmd_amount, i);
                execve(exec[i]->cmd_path, exec[i]->cmd_args, s->env);
            }
            else if (i == cmd_amount - 1)
            {
                dup2((*pipes)[(i - 1) * 2], 0);
                close_pipes_all(*pipes, cmd_amount, i);
                execve(exec[i]->cmd_path, exec[i]->cmd_args, s->env);
            }
            else
            {
                dup2((*pipes)[(i - 1) * 2], 0);
                dup2((*pipes)[i * 2 + 1], 1);
                close_pipes_all(*pipes, cmd_amount, i);
                execve(exec[i]->cmd_path, exec[i]->cmd_args, s->env);
            }
        }
        else
            i++;
    }
    mother_close_pipes_all(*pipes, cmd_amount);
    i = cmd_amount - 1;
    while (i >= 0)
    {
        waitpid((*forks)[i], &s->status, 0);
        i--;
    }
    //int i;

    // i = 0;
    // while (exec[i])
    // {
    //     printf("%d\n", i); 
    //     forks[i] = fork();
    //     if (forks[i] == 0)
    //     {    
    //         if (i == 0)
    //         {
    //             dup2(pipes[i * 2 + 1], 1);
    //             close_pipes_all(pipes, cmd_amount, i);
    //             execve(exec[i]->cmd_path, exec[i]->cmd_args, s->env);
    //         }
    //         else if (i == cmd_amount - 1)
    //         {
    //             dup2(pipes[(i - 1) * 2], 0);
    //             close_pipes_all(pipes, cmd_amount, i);
    //             execve(exec[i]->cmd_path, exec[i]->cmd_args, s->env);
    //         }
    //         else
    //         {
    //             dup2(pipes[(i - 1) * 2], 0);
    //             dup2(pipes[i * 2 + 1], 1);
    //             close_pipes_all(pipes, cmd_amount, i);
    //             execve(exec[i]->cmd_path, exec[i]->cmd_args, s->env);
    //         }
    //     }
    //     else
    //         i++;
    // }
    // i = 0;
    // while (i < cmd_amount - 1)
    // {
    //     waitpid(forks[i], NULL, 0);
    //     i++;
    // }
}

int	ft_execuator(t_state *s)
{
	t_exec	**exec;
	int		cmd_amount;
	int		err_no;
    int     *pipes;
    int     *forks;
    int     i;

    i = 0;
    pipes = NULL;
    forks = NULL;
	cmd_amount = ft_amount_cmd(s->tokens);
	exec = malloc(sizeof(t_exec *) * (cmd_amount + 1));
	ft_add_garbage(s, exec);
	exec[cmd_amount] = NULL;
	err_no = ft_init_execs(s, exec);
	if (err_no)
		return (err_no);
    ft_init_pipes(s, cmd_amount, &pipes, &forks);
    ft_lets_go(s, exec, cmd_amount, &pipes, &forks);
	return (0);
}
