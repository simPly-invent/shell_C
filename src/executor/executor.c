#include "minishell.h"
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>

static int handle_redirection(t_redir *redir)
{
    int fd = -1;

    if (!redir)
        return (0);
    if (redir->type == AST_REDIR_IN)
        fd = open(redir->file, O_RDONLY);
    else if (redir->type == AST_REDIR_OUT)
        fd = open(redir->file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    else if (redir->type == AST_REDIR_APPEND)
        fd = open(redir->file, O_WRONLY | O_CREAT | O_APPEND, 0644);
    else if (redir->type == AST_HEREDOC)
        fd = open(redir->file, O_RDONLY);

    if (fd == -1)
    {
        perror("minishell");
        return (-1);
    }
    return (fd);
}

int execute_command(t_ast *ast, t_shell *shell)
{
    if (!ast || !ast->args || !ast->args[0])
        return (0);
    
    t_redir *redir = ast->redirs;
    int saved_stdin = dup(STDIN_FILENO);
    int saved_stdout = dup(STDOUT_FILENO);
    int status = 0;

    while (redir)
    {
        int fd = handle_redirection(redir);
        if (fd == -1)
        {
            status = 1;
            break;
        }
        if (redir->type == AST_REDIR_IN || redir->type == AST_HEREDOC)
            dup2(fd, STDIN_FILENO);
        else
            dup2(fd, STDOUT_FILENO);
        close(fd);
        redir = redir->next;
    }

    if (status == 0)
    {
        if (ft_strcmp(ast->args[0], "echo") == 0) status = builtin_echo(ast, shell);
        else if (ft_strcmp(ast->args[0], "cd") == 0) status = builtin_cd(ast, shell);
        else if (ft_strcmp(ast->args[0], "pwd") == 0) status = builtin_pwd(ast, shell);
        else if (ft_strcmp(ast->args[0], "export") == 0) status = builtin_export(ast, shell);
        else if (ft_strcmp(ast->args[0], "unset") == 0) status = builtin_unset(ast, shell);
        else if (ft_strcmp(ast->args[0], "env") == 0) status = builtin_env(ast, shell);
        else if (ft_strcmp(ast->args[0], "exit") == 0) status = builtin_exit(ast, shell);
        else status = execute_external(ast, shell);
    }

    dup2(saved_stdin, STDIN_FILENO);
    dup2(saved_stdout, STDOUT_FILENO);
    close(saved_stdin);
    close(saved_stdout);

    return (status);
}


static int execute_pipeline_recursive(t_ast *ast, t_shell *shell, int pipe_in)
{
    int pipe_fd[2];
    pid_t pid;
    int status;

    if (!ast)
        return (0);
    if (ast->type != AST_PIPE)
        return (execute_command(ast, shell));
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
        exit(execute_pipeline_recursive(ast->left, shell, -1));
    }
    if (pipe_in != -1)
        close(pipe_in);
    close(pipe_fd[1]);
    status = execute_pipeline_recursive(ast->right, shell, pipe_fd[0]);
    waitpid(pid, &status, 0);
    return (WIFEXITED(status) ? WEXITSTATUS(status) : 1);
}

int execute_ast(t_ast *ast, t_shell *shell)
{
    if (!ast)
        return (0);
    return (execute_pipeline_recursive(ast, shell, -1));
}
