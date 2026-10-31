#include "minishell.h"

int     builtin_pwd(t_ast *ast, t_shell *shell)
{
    char    *cwd;

    (void)ast;
    (void)shell;
    cwd = getcwd(NULL, 0);
    if (!cwd)
        return (1);
    ft_putstr_fd(cwd, STDOUT_FILENO);
    ft_putstr_fd("\n", STDOUT_FILENO);
    free(cwd);
    return (0);
}
