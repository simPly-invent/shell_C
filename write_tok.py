with open('src/parser/tokenizer.c', 'w') as f:
    f.write('# include "minishell.h"\n\n')
    f.write('int     is_metachar(char c)\n{\n    return (c == '|' || c == '<' || c == '>' || c == ' ' || c == '\\t');\n}\n\n')
    f.write('int     is_quote(char c)\n{\n    return (c == '\\\'\' || c == '\"\');\n}\n\n')
    f.write('t_token *create_token(t_token_type type, char *value)\n{\n    t_token   *token;\n\n    token = (t_token *)malloc(sizeof(t_token));\n    if (!token)\n        return (NULL);\n    token->type = type;\n    token->value = value;\n    token->next = NULL;\n    return (token);\n}\n\n')
    f.write('void    add_token(t_token **head, t_token *new)\n{\n    t_token   *tmp;\n\n    if (!*head)\n    {\n        *head = new;\n        return ;\n    }\n    tmp = *head;\n    while (tmp->next)\n        tmp = tmp->next;\n    tmp->next = new;\n}\n\n')
    f.write('void    free_tokens(t_token *tokens)\n{\n    t_token   *tmp;\n\n    while (tokens)\n    {\n        tmp = tokens->next;\n        free(tokens->value);\n        free(tokens);\n        tokens = tmp;\n    }\n}\n\n')
    f.write('t_token   *tokenize(char *input)\n{\n    (void)input;\n    return (NULL);\n}\n')
print('Done')
