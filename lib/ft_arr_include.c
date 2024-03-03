#include "../minishell.h"

int ft_arr_include(char **arr, char *str, int (*cmp)(const char *, const char *))
{
    int i;

    i = 0;
    while (arr[i])
    {
        if (cmp == NULL)
        {
            if (ft_strcmp(arr[i], str) == 0)
                return (i);
        }
        else
        {
            if (cmp(arr[i], str) == 0)
                return (i);
        }
        i++;
    }
    return (-1);
}
