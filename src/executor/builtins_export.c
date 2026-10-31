#include "minishell.h"

int     builtin_export(t_ast *ast, t_shell *shell)
{
    int i = 1;

    if (!ast || !ast->args)
        return (0);
    if (!ast->args[1])
        return (builtin_env(ast, shell));
    while (ast->args[i])
    {
        char *arg = ast->args[i];
        char *eq = ft_strchr(arg, '=');
        if (eq)
        {
            *eq = '\0';
            add_env(&shell->env, arg, eq + 1);
            *eq = '=';
        }
        else
        {
            add_env(&shell->env, arg, "");
        }
        i++;
    }
    return (0);
}
