# include "libft.h"

char    *ft_strncat(char *dst, const char *src, size_t len)
{
    size_t  i;
    size_t  j;

    i = 0;
    while (dst[i])
        i++;
    j = 0;
    while (j < len && src[j])
    {
        dst[i + j] = src[j];
        j++;
    }
    dst[i + j] = '\0';
    return (dst);
}
