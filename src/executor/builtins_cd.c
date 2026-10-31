#include "minishell.h"
#include <limits.h>

int     builtin_cd(t_ast *ast, t_shell *shell)
{
    char    *target;
    char    *cwd;

    if (!ast || !ast->args)
        return (0);
    if (!ast->args[1])
    {
        t_env *home = find_env(shell->env, "HOME");
        if (!home)
        {
            ft_putstr_fd("minishell: cd: HOME not set\n", STDERR_FILENO);
            return (1);
        }
        // Extract value after '='
        target = ft_strchr(home->value, '=') + 1;
    }
    else if (ft_strcmp(ast->args[1], "-") == 0)
    {
        if (!shell->old_pwd)
        {
            ft_putstr_fd("minishell: cd: OLDPWD not set\n", STDERR_FILENO);
            return (1);
        }
        target = shell->old_pwd;
        ft_putstr_fd(target, STDOUT_FILENO);
        ft_putstr_fd("\n", STDOUT_FILENO);
    }
    else
    {
        target = ast->args[1];
    }

    cwd = getcwd(NULL, PATH_MAX);
    if (!cwd)
        return (1);
    if (chdir(target) != 0)
    {
        ft_putstr_fd("minishell: cd: ", STDERR_FILENO);
        ft_putstr_fd(target, STDERR_FILENO);
        ft_putstr_fd(": No such file or directory\n", STDERR_FILENO);
        free(cwd);
        return (1);
    }
    if (shell->old_pwd)
        free(shell->old_pwd);
    shell->old_pwd = ft_strdup(cwd);
    free(cwd);
    return (0);
}
