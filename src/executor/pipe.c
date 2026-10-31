#include "minishell.h"

static int     execute_pipe_recursive(t_ast *ast, t_shell *shell, int pipe_in)
{
    int     pipe_fd[2];
    int     pid;
    int     status;

    if (!ast)
        return (0);
    if (ast->type != AST_PIPE)
        return (execute_ast(ast, shell));
    if (pipe(pipe_fd) == -1)
        return (1);
    pid = fork();
    if (pid == 0)
    {
        if (pipe_in != -1)
            dup2(pipe_in, STDIN_FILENO);
        dup2(pipe_fd[1], STDOUT_FILENO);
        if (pipe_in != -1)
            close(pipe_in);
        close(pipe_fd[0]);
        close(pipe_fd[1]);
        exit(execute_ast(ast->left, shell));
    }
    if (pipe_in != -1)
        close(pipe_in);
    close(pipe_fd[1]);
    status = execute_pipe_recursive(ast->right, shell, pipe_fd[0]);
    waitpid(pid, &status, 0);
    return (status);
}

int     execute_pipeline(t_ast *ast, t_shell *shell)
{
    if (!ast)
        return (0);
    return (execute_pipe_recursive(ast, shell, -1));
}
