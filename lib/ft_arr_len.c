#include "../minishell.h"

int ft_arr_len(char **arr)
{
    int i;

    i = 0;
    while (arr[i])
        i++;
    return (i);
}