#include "minishell.h"
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>

/*
** Reads lines from the terminal until the delimiter and returns the read end
** of a pipe holding them, so `cmd << EOF` gets its input like any other
** redirection.
*/
static int heredoc_fd(const char *delim)
{
    int     fd[2];
    char    buf[1024];
    size_t  len;

    if (pipe(fd) == -1)
        return (-1);
    while (1)
    {
        printf("> ");
        fflush(stdout);
        if (!fgets(buf, sizeof(buf), stdin))
            break;
        len = ft_strlen(buf);
        if (len && buf[len - 1] == '\n')
            buf[--len] = '\0';
        if (ft_strcmp(buf, (char *)delim) == 0)
            break;
        write(fd[1], buf, len);
        write(fd[1], "\n", 1);
    }
    close(fd[1]);
    return (fd[0]);
}

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
        fd = redir->heredoc_fd;

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


static int run_last_command(t_ast *ast, t_shell *shell, int pipe_in)
{
    pid_t pid;
    int status;

    if (pipe_in == -1)
        return (execute_command(ast, shell));
    pid = fork();
    if (pid == 0)
    {
        dup2(pipe_in, STDIN_FILENO);
        close(pipe_in);
        exit(execute_command(ast, shell));
    }
    close(pipe_in);
    if (pid < 0)
        return (1);
    waitpid(pid, &status, 0);
    return (WIFEXITED(status) ? WEXITSTATUS(status) : 1);
}

/*
** a | b | c is parsed as PIPE(a, PIPE(b, c)): every left node is a command,
** it runs in a child writing to the pipe; the right side is recursed with the
** read end as its stdin. The pipeline status is the status of the last command.
*/
static int execute_pipeline_recursive(t_ast *ast, t_shell *shell, int pipe_in)
{
    int pipe_fd[2];
    pid_t pid;
    int status;
    int right_status;

    if (!ast)
        return (0);
    if (ast->type != AST_PIPE)
        return (run_last_command(ast, shell, pipe_in));
    if (pipe(pipe_fd) == -1)
        return (1);
    pid = fork();
    if (pid == 0)
    {
        if (pipe_in != -1)
        {
            dup2(pipe_in, STDIN_FILENO);
            close(pipe_in);
        }
        dup2(pipe_fd[1], STDOUT_FILENO);
        close(pipe_fd[0]);
        close(pipe_fd[1]);
        exit(execute_command(ast->left, shell));
    }
    if (pipe_in != -1)
        close(pipe_in);
    close(pipe_fd[1]);
    right_status = execute_pipeline_recursive(ast->right, shell, pipe_fd[0]);
    waitpid(pid, &status, 0);
    return (right_status);
}

/*
** Heredocs are read before anything is forked so the lines come from the
** terminal in order, even in the middle of a pipeline.
*/
static int prepare_heredocs(t_ast *ast)
{
    t_redir *r;

    if (!ast)
        return (0);
    if (ast->type == AST_PIPE)
        return (prepare_heredocs(ast->left) || prepare_heredocs(ast->right));
    r = ast->redirs;
    while (r)
    {
        if (r->type == AST_HEREDOC)
        {
            r->heredoc_fd = heredoc_fd(r->file);
            if (r->heredoc_fd == -1)
                return (1);
        }
        r = r->next;
    }
    return (0);
}

int execute_ast(t_ast *ast, t_shell *shell)
{
    if (!ast)
        return (0);
    if (prepare_heredocs(ast))
        return (1);
    return (execute_pipeline_recursive(ast, shell, -1));
}
