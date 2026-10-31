# include "libft.h"

char    *ft_strjoin3_free(char *s1, char *s2, char *s3)
{
    char    *result;
    size_t  len1;
    size_t  len2;
    size_t  len3;
    size_t  i;
    size_t  j;
    size_t  k;

    len1 = s1 ? ft_strlen(s1) : 0;
    len2 = s2 ? ft_strlen(s2) : 0;
    len3 = s3 ? ft_strlen(s3) : 0;
    result = (char *)malloc(len1 + len2 + len3 + 1);
    if (!result)
    {
        free(s1);
        free(s2);
        free(s3);
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
    k = 0;
    while (k < len3)
    {
        result[i + j + k] = s3[k];
        k++;
    }
    result[i + j + k] = '\0';
    free(s1);
    free(s2);
    free(s3);
    return (result);
}
