#include "kernel/libc.h"


int	ft_strcmp(const char *s1, const char *s2)
{
	unsigned int	i;

	i = 0;
	while (s1[i] == s2[i] && s1[i])
	{
		i++;
	}
	return ((unsigned char)s1[i] - (unsigned char)s2[i]);
}

int	ft_strncmp(const char *s1, const char *s2, size_t n)
{
	unsigned int	i;

	i = 0;
	if (n == 0)
		return (0);
	while (s1[i] == s2[i] && (i < n - 1) && s1[i])
	{
		i++;
	}
	return ((unsigned char)s1[i] - (unsigned char)s2[i]);
}

int	ft_atoi(const char *nptr)
{
	int	i;
	int	nb;
	int	sign;

	i = 0;
	nb = 0;
	sign = 1;
	while ((nptr[i] > 8 && nptr[i] < 14) || nptr[i] == 32)
		i++;
	if (nptr[i] == '-')
	{
		sign *= -1;
		i++;
	}
	else if (nptr[i] == '+')
		i++;
	while (nptr[i] >= '0' && nptr[i] <= '9')
	{
		nb = nb * 10 + (nptr[i] - '0');
		i++;
	}
	return (nb * sign);
}

size_t	ft_strlen(const char *s)
{
	size_t			len;

	len = 0;
	while (s[len] != '\0')
	{
		len++;
	}
	return (len);
}

char	*ft_strtrim(char const *s1, char const *set)
{
	int		i;
	int		j;
	char	*str;

	if (!s1)
		return (NULL);
	str = (char *)s1;
	i = 0;
	j = ft_strlen(str) - 1;
	while (str[i] && ft_strrchr(set, (int)str[i]))
		i++;
	while (j >= 0 && ft_strrchr(set, (int)str[j]))
		j--;
	if (i > j)
	{
		str[0] = '\0';
		return (str);
	}
	str[j + 1] = '\0';
	return (str + i);
}

char	*ft_strrchr(const char *s, int c)
{
	char		*str;
	char		find;
	size_t		i;

	str = (char *)s;
	find = (char)c;
	i = ft_strlen(str);
	if (c == '\0')
		return (str + ft_strlen(str));
	while (i--)
	{
		if (str[i] == find)
			return (str + i);
	}
	return (NULL);
}
