# include "libft.h"

static size_t ft_count_words(char const *s, char c)
{
    size_t  count;
    int     in_word;

    count = 0;
    in_word = 0;
    while (*s)
    {
        if (*s != c && !in_word)
        {
            in_word = 1;
            count++;
        }
        else if (*s == c)
            in_word = 0;
        s++;
    }
    return (count);
}

static char   *ft_word_dup(char const *s, char c)
{
    size_t  len;
    char    *word;

    len = 0;
    while (s[len] && s[len] != c)
        len++;
    word = (char *)malloc(len + 1);
    if (!word)
        return (NULL);
    ft_strlcpy(word, s, len + 1);
    return (word);
}

static void ft_free_split(char **arr, size_t count)
{
    while (count--)
        free(arr[count]);
    free(arr);
}

char    **ft_split(char const *s, char c)
{
    char    **result;
    size_t  count;
    size_t  i;

    if (!s)
        return (NULL);
    count = ft_count_words(s, c);
    result = (char **)malloc(sizeof(char *) * (count + 1));
    if (!result)
        return (NULL);
    i = 0;
    while (*s)
    {
        if (*s != c)
        {
            result[i] = ft_word_dup(s, c);
            if (!result[i])
            {
                ft_free_split(result, i);
                return (NULL);
            }
            i++;
            while (*s && *s != c)
                s++;
        }
        else
            s++;
    }
    result[i] = NULL;
    return (result);
}
