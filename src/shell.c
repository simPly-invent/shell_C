#include "minishell.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <readline/readline.h>
#include <readline/history.h>

void    shell_init(t_shell *shell, char **envp)
{
    shell->env = copy_env(envp);
    shell->env_array = get_env_array(shell->env);
    shell->last_exit_code = 0;
    shell->should_exit = 0;
    shell->tokens = NULL;
    shell->ast = NULL;
    shell->old_pwd = NULL;
    setup_signals();
}

void    shell_loop(t_shell *shell)
{
    char    buffer[1024];
    char    *input;

    while (!shell->should_exit)
    {
        printf("minishell$ ");
        fflush(stdout);
        if (!fgets(buffer, 1024, stdin))
        {
            break;
        }
        
        // Remove newline
        int len = 0;
        while (buffer[len] && buffer[len] != '\n') len++;
        buffer[len] = '\0';
        input = ft_strdup(buffer);

        if (!input || !*input)
        {
            free(input);
            continue;
        }
        
        shell->tokens = tokenize(input);
        shell->ast = parse(shell->tokens);
        expand_ast(shell->ast, shell);
        shell->last_exit_code = execute_ast(shell->ast, shell);
        
        free_ast(shell->ast);
        free_tokens(shell->tokens);
        free(input);
    }
}


void    shell_cleanup(t_shell *shell)
{
    free_env(shell->env);
    if (shell->env_array)
    {
        for (int i = 0; shell->env_array[i]; i++)
            free(shell->env_array[i]);
        free(shell->env_array);
    }
    if (shell->old_pwd)
        free(shell->old_pwd);
}
