#include "minishell.h"

int     execute_redirections(t_redir *redirs, t_shell *shell)
{
    int     fd;

    while (redirs)
    {
        if (redirs->type == AST_REDIR_IN)
        {
            fd = open(redirs->file, O_RDONLY);
            if (fd == -1)
                return (1);
            dup2(fd, STDIN_FILENO);
            close(fd);
        }
        else if (redirs->type == AST_REDIR_OUT)
        {
            fd = open(redirs->file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
            if (fd == -1)
                return (1);
            dup2(fd, STDOUT_FILENO);
            close(fd);
        }
        else if (redirs->type == AST_REDIR_APPEND)
        {
            fd = open(redirs->file, O_WRONLY | O_CREAT | O_APPEND, 0644);
            if (fd == -1)
                return (1);
            dup2(fd, STDOUT_FILENO);
            close(fd);
        }
        else if (redirs->type == AST_HEREDOC)
        {
            if (setup_heredoc(redirs, shell) == -1)
                return (1);
        }
        redirs = redirs->next;
    }
    return (0);
}

int     setup_heredoc(t_redir *redir, t_shell *shell)
{
    (void)shell;
    int     fd;
    char    *line;
    char    *content;

    fd = open("heredoc_tmp", O_WRONLY | O_CREAT | O_TRUNC, 0600);
    if (fd == -1)
        return (-1);
    content = NULL;
    while (1)
    {
        line = readline(". ");
        if (!line || (strcmp(line, redir->delimiter) == 0))
        {
            free(line);
            break;
        }
        write(fd, line, ft_strlen(line));
        write(fd, "\n", 1);
        free(line);
    }
    close(fd);
    int in_fd = open("heredoc_tmp", O_RDONLY);
    if (in_fd == -1)
        return (-1);
    dup2(in_fd, STDIN_FILENO);
    close(in_fd);
    unlink("heredoc_tmp");
    return (0);
}
