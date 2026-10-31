#include "minishell.h"
#include <stdlib.h>

static int is_numeric(char *s)
{
    int i = 0;
    if (!s || !s[i]) return (0);
    if (s[i] == '-' || s[i] == '+') i++;
    if (!s[i]) return (0);
    while (s[i])
    {
        if (!ft_isdigit(s[i])) return (0);
        i++;
    }
    return (1);
}

int     builtin_exit(t_ast *ast, t_shell *shell)
{
    int status = 0;

    if (ast && ast->args && ast->args[1])
    {
        if (!is_numeric(ast->args[1]))
        {
            ft_putstr_fd("minishell: numeric argument required", STDERR_FILENO);
            ft_putstr_fd("\n", STDERR_FILENO);
            return (1);
        }
        status = ft_atoi(ast->args[1]);
    }
    shell->should_exit = 1;
    return (status);
}
