#include "minishell.h"
#include <stdio.h>

char *get_env_value(t_shell *shell, char *key)
{
    t_env *env = find_env(shell->env, key);
    if (!env)
        return (ft_strdup(""));
    
    // The environment is stored as "KEY=VALUE"
    // We need to find the first '=' to extract the value
    char *val = ft_strchr(env->value, '=');
    if (!val)
        return (ft_strdup(""));
    
    return (ft_strdup(val + 1));
}

char *expand_word(char *word, t_shell *shell, t_quote_type quote_type)
{
    if (!word)
        return (NULL);
    
    // No expansion in single quotes
    if (quote_type == QUOTE_SINGLE)
        return (ft_strdup(word));
    
    int i = 0;
    char *res = ft_strdup("");
    char *val;

    while (word[i])
    {
        if (word[i] == '$')
        {
            i++;
            if (word[i] == '$')
            {
                char *tmp = ft_strjoin(res, "$");
                free(res);
                res = tmp;
                i++;
            }
            else if (word[i] == '?')
            {
                char *exit_str = ft_itoa(shell->last_exit_code);
                char *tmp = ft_strjoin(res, exit_str);
                free(res);
                free(exit_str);
                res = tmp;
                i++;
            }
            else
            {
                int start = i;
                while (word[i] && (ft_isalnum(word[i]) || word[i] == '_'))
                    i++;
                char *key = ft_substr(word, start, i - start);
                val = get_env_value(shell, key);
                char *tmp = ft_strjoin(res, val);
                free(res);
                free(key);
                free(val);
                res = tmp;
            }
        }
        else
        {
            char tmp[2] = {word[i], '\0'};
            char *new_res = ft_strjoin(res, tmp);
            free(res);
            res = new_res;
            i++;
        }
    }
    return (res);
}

void expand_ast(t_ast *ast, t_shell *shell)
{
    if (!ast)
        return;
    if (ast->type == AST_PIPE)
    {
        expand_ast(ast->left, shell);
        expand_ast(ast->right, shell);
    }
    else if (ast->type == AST_CMD && ast->args)
    {
        int i = 0;
        while (ast->args[i])
        {
            t_quote_type qt = QUOTE_NONE;
            if (ast->quote_types)
                qt = ast->quote_types[i];
            char *expanded = expand_word(ast->args[i], shell, qt);
            free(ast->args[i]);
            ast->args[i] = expanded;
            i++;
        }
    }
}