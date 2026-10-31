#include "minishell.h"
#include <stdio.h>

static t_ast *create_ast_node(t_ast_type type)
{
    t_ast *node = calloc(1, sizeof(t_ast));
    if (!node)
        return (NULL);
    node->type = type;
    return (node);
}

static t_ast_type map_token_to_ast(t_token_type type)
{
    if (type == TOKEN_REDIR_IN) return (AST_REDIR_IN);
    if (type == TOKEN_REDIR_OUT) return (AST_REDIR_OUT);
    if (type == TOKEN_REDIR_APPEND) return (AST_REDIR_APPEND);
    if (type == TOKEN_HEREDOC) return (AST_HEREDOC);
    return (AST_CMD);
}

t_ast *parse_command(t_token **tokens)
{
    t_ast *node = create_ast_node(AST_CMD);
    if (!node)
        return (NULL);
    
    t_redir *redir_head = NULL;
    t_redir *redir_tail = NULL;
    char **args = NULL;
    t_quote_type *quote_types = NULL;
    int arg_count = 0;

    while (*tokens && (*tokens)->type != TOKEN_PIPE)
    {
        if ((*tokens)->type == TOKEN_WORD)
        {
            char *val = ft_strdup((*tokens)->value);
            args = ft_realloc_args(args, arg_count, val);
            quote_types = realloc(quote_types, (arg_count + 1) * sizeof(t_quote_type));
            quote_types[arg_count] = (*tokens)->quote_type;
            arg_count++;
        }
        else if ((*tokens)->type == TOKEN_REDIR_IN || (*tokens)->type == TOKEN_REDIR_OUT || 
                 (*tokens)->type == TOKEN_REDIR_APPEND || (*tokens)->type == TOKEN_HEREDOC)
        {
            t_ast_type type = map_token_to_ast((*tokens)->type);
            *tokens = (*tokens)->next;
            if (!*tokens || (*tokens)->type != TOKEN_WORD)
            {
                // Cleanup to avoid leaks
                if (args)
                {
                    for (int i = 0; args[i]; i++) free(args[i]);
                    free(args);
                }
                t_redir *r = redir_head;
                while (r)
                {
                    t_redir *t = r;
                    r = r->next;
                    free(t->file);
                    free(t->delimiter);
                    free(t);
                }
                free(node);
                return (NULL);
            }
            
            t_redir *redir = calloc(1, sizeof(t_redir));
            redir->type = type;
            redir->file = ft_strdup((*tokens)->value);
            
            if (!redir_head)
                redir_head = redir;
            else
                redir_tail->next = redir;
            redir_tail = redir;
        }
        *tokens = (*tokens)->next;
    }
    node->args = args;
    node->quote_types = quote_types;
    node->redirs = redir_head;
    return (node);
}

t_ast *parse_pipeline(t_token **tokens)
{
    t_ast *left = parse_command(tokens);
    if (!left)
        return (NULL);
    if (*tokens && (*tokens)->type == TOKEN_PIPE)
    {
        (*tokens)++;
        t_ast *node = create_ast_node(AST_PIPE);
        node->left = left;
        node->right = parse_pipeline(tokens);
        return (node);
    }
    return (left);
}

t_ast *parse(t_token *tokens)
{
    t_token *token_copy = tokens;
    t_ast *res = parse_pipeline(&token_copy);
    return (res);
}

void free_ast(t_ast *ast)
{
    if (!ast)
        return;
    if (ast->type == AST_PIPE)
    {
        free_ast(ast->left);
        free_ast(ast->right);
    }
    else if (ast->type == AST_CMD)
    {
        if (ast->args)
        {
            int i = 0;
            while (ast->args[i])
                free(ast->args[i++]);
            free(ast->args);
        }
        t_redir *redir = ast->redirs;
        while (redir)
        {
            t_redir *tmp = redir;
            redir = redir->next;
            free(tmp->file);
            free(tmp->delimiter);
            free(tmp);
        }
    }
    free(ast);
}
