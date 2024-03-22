#include "minishell.h"

void    ft_error(int err, char *str, int throw_exit)
{
    if (err == ERR_CMD_NOT_FOUND)
    {
        if (str)
            printf("minishell: %s: command not found\n", str);
        else
            printf("minishell: command not found\n");
    }
    else if (err == ERR_UNEXPECTED_TOKEN)
    {
        if (str)
            printf("minishell: syntax error near unexpected token `%s'\n", str);
        else
            printf("minishell: syntax error near unexpected token\n");
    }
	else if (err == ERR_EMPTY_COMMAND)
	{
		if (str)
			printf("minishell: %s: command not found\n", str);
		else
			printf("minishell: command not found\n");
	}
    if (throw_exit)
        exit(1);
}