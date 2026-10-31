#include "minishell.h"

int     is_builtin(char *cmd)
{
    if (!cmd)
        return (0);
    if (strcmp(cmd, "echo") == 0 || strcmp(cmd, "cd") == 0 || 
        strcmp(cmd, "pwd") == 0 || strcmp(cmd, "export") == 0 || 
        strcmp(cmd, "unset") == 0 || strcmp(cmd, "env") == 0 || 
        strcmp(cmd, "exit") == 0)
        return (1);
    return (0);
}

int     execute_builtin(t_ast *ast, t_shell *shell)
{
    if (!ast || !ast->args || !ast->args[0])
        return (1);
    if (strcmp(ast->args[0], "echo") == 0)
        return (builtin_echo(ast, shell));
    if (strcmp(ast->args[0], "cd") == 0)
        return (builtin_cd(ast, shell));
    if (strcmp(ast->args[0], "pwd") == 0)
        return (builtin_pwd(ast, shell));
    if (strcmp(ast->args[0], "export") == 0)
        return (builtin_export(ast, shell));
    if (strcmp(ast->args[0], "unset") == 0)
        return (builtin_unset(ast, shell));
    if (strcmp(ast->args[0], "env") == 0)
        return (builtin_env(ast, shell));
    if (strcmp(ast->args[0], "exit") == 0)
        return (builtin_exit(ast, shell));
    return (1);
}
