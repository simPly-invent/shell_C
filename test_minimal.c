
#include "includes/minishell.h"
#include <stdio.h>
#include <stdlib.h>

int main() {
    printf("MINIMAL: Start\n");
    t_shell shell;
    char *envp[] = {"PATH=/bin", NULL};
    
    printf("MINIMAL: Calling shell_init...\n");
    shell_init(&shell, envp);
    printf("MINIMAL: shell_init done\n");
    
    char *input = "echo hello";
    printf("MINIMAL: Calling tokenize...\n");
    shell.tokens = tokenize(input);
    printf("MINIMAL: tokenize done\n");
    
    if (shell.tokens) {
        printf("MINIMAL: First token: %s\n", shell.tokens->value);
    }
    
    return 0;
}
