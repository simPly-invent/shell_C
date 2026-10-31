#include "minishell.h"

int     builtin_echo(t_ast *ast, t_shell *shell)
{
    (void)shell;
    int i = 1;
    int newline = 1;

    if (!ast || !ast->args)
        return (1);
    if (ast->args[i] && ft_strcmp(ast->args[i], "-n") == 0)
    {
        newline = 0;
        i++;
    }
    while (ast->args[i])
    {
        ft_putstr_fd(ast->args[i], STDOUT_FILENO);
        if (ast->args[i + 1])
            ft_putstr_fd(" ", STDOUT_FILENO);
        i++;
    }
    if (newline)
        ft_putstr_fd("\n", STDOUT_FILENO);
    return (0);
}
