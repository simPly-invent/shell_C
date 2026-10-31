#include "minishell.h"

t_env *copy_env(char **envp)
{
    t_env *head = NULL;
    t_env *tail = NULL;
    int i = 0;

    if (!envp)
        return (NULL);
    while (envp[i])
    {
        t_env *new = malloc(sizeof(t_env));
        if (!new)
            return (NULL);
        new->value = ft_strdup(envp[i]);
        new->next = NULL;
        if (!head)
            head = new;
        else
            tail->next = new;
        tail = new;
        i++;
    }
    return (head);
}

void free_env(t_env *env)
{
    t_env *tmp;

    while (env)
    {
        tmp = env;
        env = env->next;
        free(tmp->value);
        free(tmp);
    }
}

char **get_env_array(t_env *env)
{
    char **array;
    t_env *curr = env;
    int i = 0;

    while (curr)
    {
        i++;
        curr = curr->next;
    }
    array = malloc(sizeof(char *) * (i + 1));
    if (!array)
        return (NULL);
    curr = env;
    i = 0;
    while (curr)
    {
        array[i++] = ft_strdup(curr->value);
        curr = curr->next;
    }
    array[i] = NULL;
    return (array);
}

t_env *find_env(t_env *env, char *key)
{
    if (!env || !key) return (NULL);
    t_env *curr = env;
    int key_len = 0;

    while (key[key_len]) key_len++;

    while (curr)
    {
        if (ft_strncmp(curr->value, key, key_len) == 0 && curr->value[key_len] == '=')
            return (curr);
        curr = curr->next;
    }
    return (NULL);
}

void add_env(t_env **env, char *key, char *value)
{
    t_env *existing = find_env(*env, key);
    if (existing)
    {
        char *new_val = ft_strjoin(key, "=");
        char *final_val = ft_strjoin_free(new_val, value);
        free(existing->value);
        existing->value = final_val;
    }
    else
    {
        t_env *new = malloc(sizeof(t_env));
        if (!new) return;
        char *new_val = ft_strjoin(key, "=");
        new->value = ft_strjoin_free(new_val, value);
        new->next = *env;
        *env = new;
    }
}


int     ft_strcmp_env(t_env *a, t_env *b)
{
    return (ft_strcmp(a->value, b->value));
}

void    sort_env(t_env **env)
{
    if (!env || !*env || !(*env)->next)
        return;
    
    t_env *head = *env;
    int swapped;
    t_env *ptr1;
    t_env *lptr = NULL;

    do {
        swapped = 0;
        ptr1 = head;
        while (ptr1->next != lptr) {
            if (ft_strcmp(ptr1->value, ptr1->next->value) > 0) {
                char *tmp_val = ptr1->value;
                ptr1->value = ptr1->next->value;
                ptr1->next->value = tmp_val;
                swapped = 1;
            }
            ptr1 = ptr1->next;
        }
        lptr = ptr1;
    } while (swapped);
}
