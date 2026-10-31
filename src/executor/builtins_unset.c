#include "minishell.h"

int     builtin_unset(t_ast *ast, t_shell *shell)
{
    int i = 1;
    t_env *curr;
    t_env *prev = NULL;

    if (!ast || !ast->args)
        return (0);
    while (ast->args[i])
    {
        curr = shell->env;
        prev = NULL;
        while (curr)
        {
            if (strncmp(curr->value, ast->args[i], ft_strlen(ast->args[i])) == 0 && 
                curr->value[ft_strlen(ast->args[i])] == '=')
            {
                if (prev)
                    prev->next = curr->next;
                else
                    shell->env = curr->next;
                free(curr->value);
                free(curr);
                break;
            }
            prev = curr;
            curr = curr->next;
        }
        i++;
    }
    return (0);
}
