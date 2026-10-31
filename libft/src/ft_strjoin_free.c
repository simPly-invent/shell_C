# include "libft.h"

char    *ft_strjoin_free(char *s1, char *s2)
{
    char    *result;
    size_t  len1;
    size_t  len2;
    size_t  i;
    size_t  j;

    len1 = s1 ? ft_strlen(s1) : 0;
    len2 = s2 ? ft_strlen(s2) : 0;
    result = (char *)malloc(len1 + len2 + 1);
    if (!result)
    {
        free(s1);
        free(s2);
        return (NULL);
    }
    i = 0;
    while (i < len1)
    {
        result[i] = s1[i];
        i++;
    }
    j = 0;
    while (j < len2)
    {
        result[i + j] = s2[j];
        j++;
    }
    result[i + j] = '\0';
    free(s1);
    free(s2);
    return (result);
}
