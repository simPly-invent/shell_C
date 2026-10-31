#include "minishell.h"
#include <stdio.h>
#include <unistd.h>
#include <errno.h>

static char *resolve_path(t_shell *shell, char *cmd)
{
    if (!cmd) return (NULL);
    if (cmd[0] == '/' || cmd[0] == '~')
        return (ft_strdup(cmd));
    
    t_env *env = find_env(shell->env, "PATH");
    if (!env || !env->value) return (NULL);
    
    char *path_val = ft_strdup(env->value);
    if (!path_val) return (NULL);
    
    // Handle the 'PATH=' prefix if present
    char *start = path_val;
    if (start[0] == 'P' && start[1] == 'A' && start[2] == 'T' && start[3] == 'H' && start[4] == '=')
        start += 5;
    
    char *token = ft_strtok(start, ":");
    while (token)
    {
        char *full_path = ft_strjoin(token, "/");
        if (!full_path) break;
        char *final_path = ft_strjoin(full_path, cmd);
        free(full_path);
        if (final_path && access(final_path, X_OK) == 0)
        {
            free(path_val);
            return (final_path);
        }
        free(final_path);
        token = ft_strtok(NULL, ":");
    }
    free(path_val);
    return (NULL);
}

int execute_external(t_ast *ast, t_shell *shell)
{
    pid_t pid = fork();
    if (pid == 0)
    {
        char *path = resolve_path(shell, ast->args[0]);
        if (!path)
        {
            ft_putstr_fd("minishell: command not found: ", STDERR_FILENO);
            ft_putstr_fd(ast->args[0], STDERR_FILENO);
            ft_putstr_fd("\n", STDERR_FILENO);
            exit(127);
        }
        execve(path, ast->args, shell->env_array);
        perror("execve");
        free(path);
        exit(1);
    }
    else if (pid < 0)
        return (1);
    else
    {
        int status;
        waitpid(pid, &status, 0);
        return (WIFEXITED(status) ? WEXITSTATUS(status) : 1);
    }
}
