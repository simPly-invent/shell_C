#include "minishell.h"

int     builtin_env(t_ast *ast, t_shell *shell)
{
    (void)ast;
    sort_env(&shell->env);
    t_env *curr = shell->env;

    if (!curr)
        return (0);
    while (curr)
    {
        ft_putstr_fd(curr->value, STDOUT_FILENO);
        ft_putstr_fd("\n", STDOUT_FILENO);
        curr = curr->next;
    }
    return (0);
}
