
#include "includes/minishell.h"
#include <stdio.h>
#include <stdlib.h>

int main() {
    t_shell shell;
    char *envp[] = {"PATH=/bin:/usr/bin", NULL};
    
    printf("CORE: Starting test...\n");
    shell_init(&shell, envp);
    
    char *input = "echo hello world";
    printf("Testing input: %s\n", input);
    
    printf("TRACE: Tokenizing...\n");
    shell.tokens = tokenize(input);
    if (!shell.tokens) {
        printf("Tokenization failed\n");
        return 1;
    }
    
    printf("TRACE: Parsing...\n");
    shell.ast = parse(shell.tokens);
    if (!shell.ast) {
        printf("Parsing failed\n");
        return 1;
    }
    
    printf("TRACE: Expanding...\n");
    expand_ast(shell.ast, &shell);
    printf("TRACE: Executing...\n");
    int result = execute_ast(shell.ast, &shell);
    printf("Execution result: %d\n", result);
    
    free_tokens(shell.tokens);
    free_ast(shell.ast);
    shell_cleanup(&shell);
    
    return 0;
}
