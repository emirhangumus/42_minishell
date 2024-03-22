#include "minishell.h"

void    ft_error(int err, char *str, int throw_exit)
{
    if (err == ERR_CMD_NOT_FOUND)
    {
        if (str)
            dprintf(2, "minishell: %s: command not found\n", str);
        else
            dprintf(2, "minishell: command not found\n");
    }
	else if (err == ERR_IS_A_DIRECTORY)
	{
		if (str)
			dprintf(2, "minishell: %s: %s\n", str, strerror(EISDIR));
		else
			dprintf(2, "minishell: is a directory\n");
	}
	else if (err == ERR_PERMISSION_DENIED)
	{
		if (str)
			dprintf(2, "minishell: %s: %s\n", str, strerror(EACCES));
		else
			dprintf(2, "minishell: permission denied\n");
	}
    else if (err == ERR_UNEXPECTED_TOKEN)
    {
        if (str)
            dprintf(2, "minishell: syntax error near unexpected token `%s'\n", str);
        else
            dprintf(2, "minishell: syntax error near unexpected token\n");
    }
	else if (err == ERR_EMPTY_COMMAND)
	{
		if (str)
			dprintf(2, "minishell: %s: command not found\n", str);
		else
			dprintf(2, "minishell: command not found\n");
	}
    if (throw_exit)
        exit(1);
}