/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execuator.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: burkaya <burkaya@student.42istanbul.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/29 14:13:59 by burkaya           #+#    #+#             */
/*   Updated: 2024/03/01 00:13:40 by burkaya          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char    *get_env(char **env, char *key)
{
    int     i;
    size_t  len;

    i = 0;
    len = ft_strlen(key);
    while (env[i])
    {
        if (ft_strncmp(env[i], key, len) == 0 && env[i][len] == '=')
            return (env[i] + len + 1);
        i++;
    }
    return (NULL);
}

char	*ft_get_cmd_path(t_token *start_token, t_state *s)
{
    char    *cmd_path;
    char    **paths;
    char    *path;
    int     i;
    
    i = 0;
    path = get_env(s->env, "PATH");
    paths = ft_split(path, ':', s);
    while (paths[i])
    {
        cmd_path = ft_strjoin(paths[i], "/", s);
        cmd_path = ft_strjoin(cmd_path, start_token->value, s);
        if (access(cmd_path, F_OK) == 0)
            return (cmd_path);
        i++;
    }
    return (NULL);
}

int	ft_find_arg_amount(t_token *tokens)
{
	int	amount;

	amount = 0;
	tokens = tokens->next;
	while (tokens)
	{
        if (tokens->type == T_ARG)
    		amount++;
        else 
            break;
		tokens = tokens->next;
	}
	return (amount);
}

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
    args[arg_amount] = NULL;
    while (i <= arg_amount)
	{
		args[i] = ft_strdup(tokens->value, s);
		tokens = tokens->next;
		i++;
	}
	
	return (args);
}

// void    ft_get_cmd(t_state *s, t_exec **exec)
// {
    
// 	while (s->tokens)
// 	{
// 		if (s->tokens->type == T_CMD)
// 		{
// 			exec->cmd_path[i] = ft_get_cmd_path(s);
// 			s->tokens = s->tokens->next;
// 		}
// 		else if (s->tokens->type == T_ARG)
// 			exec->cmd_args = ft_get_args(s, &s->tokens, exec);
//         else if (s->tokens->type == T_PIPE)
//         {
//             s->tokens = s->tokens->next;
            
//         }
// 	}
// }

int    ft_init_execs(t_state *s, t_exec **exec)
{
    int j;
    t_token *tmp;
    j = 0;
    tmp = s->tokens;
    while (tmp)
    {
        if (tmp->type == T_CMD)
        {
            exec[j] = malloc(sizeof(t_exec));
            exec[j]->cmd_path = ft_get_cmd_path(tmp, s);
            if (exec[j]->cmd_path == NULL)
                return (ERR_CMD_NOT_FOUND);
            exec[j]->cmd_args = ft_get_args(s, tmp);
            j++;
        }
        tmp = tmp->next;
    }
    return (0);
}

int ft_amount_cmd(t_token *tokens)
{
    int amount;

    amount = 0;
    while (tokens)
    {
        if (tokens->type == T_CMD)
            amount++;
        tokens = tokens->next;
    }
    return (amount);
}

int	ft_execuator(t_state *s)
{
	t_exec  **exec;
    int    cmd_amount;
    int    i;
    int     err_no;
	
	i = 0;
	exec = malloc(sizeof(t_exec *) * (i + 1));
    cmd_amount = ft_amount_cmd(s->tokens);
    exec[cmd_amount] = NULL;
	err_no = ft_init_execs(s, exec);
    if (err_no)
        return (err_no);
	if (pipe(s->fd) == -1)
		return (ERR_PIPE_INIT);
    while (i < cmd_amount)
    {
        s->pid = fork();
        if (s->pid == 0)
        {
            if (i == 0 && cmd_amount > 1) // ilk komut ve birden fazla komut varsa
            {
                close(s->fd[0]);
                dup2(s->fd[1], 1);
            }
            else if (i == 0 && cmd_amount == 1) // tek komut varsa
            {
                close(s->fd[0]);
                close(s->fd[1]);
            }
            else if (i == cmd_amount - 1) // son komut
            {
                close(s->fd[1]);
                dup2(s->fd[0], 0);
            }
            else // ara komutlar
            {
                dup2(s->fd[0], 0);
                dup2(s->fd[1], 1);
            }
            printf("cmd_path: %s\n", exec[i]->cmd_path);
            execve(exec[i]->cmd_path, exec[i]->cmd_args, s->env);
        }
        else
        {
            waitpid(s->pid, &s->status, 0);
            if (i == 0 && cmd_amount == 1) // tek komut varsa
            {
                i++;
                continue ;
            }
            else if (i == 0 && cmd_amount > 1) // ilk komut ve birden fazla komut varsa
                close(s->fd[1]);
            else if (i == cmd_amount - 1) // son komut
                close(s->fd[0]);
            else // ara komutlar
            {
                close(s->fd[0]);
                close(s->fd[1]);
            }
        }
        i++;
    }
    return (0);
}

