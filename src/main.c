#include "minishell.h"
#include <stdlib.h>

int     main(int argc, char **argv, char **envp)
{
    t_shell   *shell;

    (void)argc;
    (void)argv;
    shell = malloc(sizeof(t_shell));
    if (!shell)
        return (1);
    shell_init(shell, envp);
    setup_signals();
    shell_loop(shell);
    shell_cleanup(shell);
    int exit_code = shell->last_exit_code;
    free(shell);
    return (exit_code);
}
