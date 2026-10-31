#include "minishell.h"

void    print_error(char *cmd, char *msg)
{
    if (!cmd || !msg)
        return;
    fprintf(stderr, "minishell: %s: %s\n", cmd, msg);
}

void    print_error2(char *cmd, char *arg, char *msg)
{
    if (!cmd || !arg || !msg)
        return;
    fprintf(stderr, "minishell: %s: %s: %s\n", cmd, arg, msg);
}

char    **ft_realloc_args(char **args, int count, char *val)
{
    char    **new_args;
    int     i;

    new_args = malloc(sizeof(char *) * (count + 2));
    if (!new_args)
        return (NULL);
    i = 0;
    while (i < count && args)
    {
        new_args[i] = args[i];
        i++;
    }
    new_args[i] = val;
    new_args[i + 1] = NULL;
    free(args);
    return (new_args);
}

char    *ft_strtok(char *s, const char *delim)
{
    static char    *last;
    char           *start;

    if (s)
        last = s;
    else if (!last)
        return (NULL);
    while (*last && ft_strchr(delim, *last))
        last++;
    if (!*last)
        return (NULL);
    start = last;
    while (*last && !ft_strchr(delim, *last))
        last++;
    if (*last)
        *last++ = '\0';
    return (start);
}


char    *get_current_cwd(void)
{
    char    *cwd;

    cwd = getcwd(NULL, 0);
    if (!cwd)
        return (ft_strdup("[unknown]"));
    return (cwd);
}
