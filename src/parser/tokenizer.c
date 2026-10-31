#include "minishell.h"
#include <stdio.h>

t_token *create_token(t_token_type type, char *value, t_quote_type quote_type)
{
    t_token *token = malloc(sizeof(t_token));
    if (!token)
        return (NULL);
    token->type = type;
    token->value = value ? ft_strdup(value) : NULL;
    token->quote_type = quote_type;
    token->next = NULL;
    return (token);
}

void add_token(t_token **head, t_token *new)
{
    if (!new)
        return;
    if (!*head)
    {
        *head = new;
        return;
    }
    t_token *curr = *head;
    while (curr->next)
        curr = curr->next;
    curr->next = new;
}

t_token *tokenize(char *input)
{
    t_token *head = NULL;
    int i = 0;

    if (!input)
        return (NULL);
    while (input[i])
    {
        while (input[i] == ' ' || input[i] == '\t')
            i++;
        if (!input[i])
            break;
        if (input[i] == '|' || input[i] == '<' || input[i] == '>')
        {
            if (input[i] == '|')
                add_token(&head, create_token(TOKEN_PIPE, "|", QUOTE_NONE));
            else if (input[i] == '<' && input[i + 1] == '<')
            {
                add_token(&head, create_token(TOKEN_HEREDOC, "<<", QUOTE_NONE));
                i++;
            }
            else if (input[i] == '<')
                add_token(&head, create_token(TOKEN_REDIR_IN, "<", QUOTE_NONE));
            else if (input[i] == '>' && input[i + 1] == '>')
            {
                add_token(&head, create_token(TOKEN_REDIR_APPEND, ">>", QUOTE_NONE));
                i++;
            }
            else if (input[i] == '>')
                add_token(&head, create_token(TOKEN_REDIR_OUT, ">", QUOTE_NONE));
            i++;
        }
        else
        {
            char *word = malloc(1024);
            int j = 0;
            t_quote_type quote_type = QUOTE_NONE;
            while (input[i] && input[i] != ' ' && input[i] != '\t' && 
                   input[i] != '|' && input[i] != '<' && input[i] != '>')
            {
                if (input[i] == '\'')
                {
                    if (quote_type == QUOTE_NONE)
                        quote_type = QUOTE_SINGLE;
                    i++;
                    while (input[i] && input[i] != '\'')
                        word[j++] = input[i++];
                    if (input[i] == '\'')
                        i++;
                }
                else if (input[i] == '"')
                {
                    if (quote_type == QUOTE_NONE)
                        quote_type = QUOTE_DOUBLE;
                    i++;
                    while (input[i] && input[i] != '"')
                        word[j++] = input[i++];
                    if (input[i] == '"')
                        i++;
                }
                else
                    word[j++] = input[i++];
            }
            word[j] = '\0';
            add_token(&head, create_token(TOKEN_WORD, word, quote_type));
            free(word);
        }
    }
    return (head);
}

void free_tokens(t_token *tokens)
{
    while (tokens)
    {
        t_token *tmp = tokens;
        tokens = tokens->next;
        free(tmp->value);
        free(tmp);
    }
}
