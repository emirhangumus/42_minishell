#include "../minishell.h"

char    **ft_dup_arr(char **arr)
{
    char	**dup;
    int		i;

    i = 0;
    while (arr[i])
        i++;
    dup = malloc(sizeof(char *) * (i + 1));
    if (dup == NULL)
        return (NULL);
    i = 0;
    while (arr[i])
    {
        dup[i] = ft_strdup(arr[i], NULL);
        i++;
    }
    dup[i] = NULL;
    return (dup);
}
