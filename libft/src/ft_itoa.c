# include "libft.h"

static int  ft_intlen(int n)
{
    int len;

    len = 0;
    if (n <= 0)
        len++;
    while (n != 0)
    {
        n /= 10;
        len++;
    }
    return (len);
}

char    *ft_itoa(int n)
{
    char    *str;
    int     len;
    long    nb;

    nb = n;
    len = ft_intlen(n);
    str = (char *)malloc(len + 1);
    if (!str)
        return (NULL);
    str[len] = '\0';
    if (nb == 0)
        str[0] = '0';
    if (nb < 0)
    {
        str[0] = '-';
        nb = -nb;
    }
    while (nb > 0)
    {
        str[--len] = (nb % 10) + '0';
        nb /= 10;
    }
    return (str);
}
