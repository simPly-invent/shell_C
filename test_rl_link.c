
#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>

int main() {
    printf("RL_TEST: Start\n");
    char *input = readline("prompt> ");
    printf("RL_TEST: Received: %s\n", input);
    return 0;
}
