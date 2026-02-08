#ifndef LIBC_H
# define LIBC_H

# include <stddef.h>
# include <stdint.h>

size_t		ft_strlen(const char *s);
int			ft_strncmp(const char *s1, const char *s2, size_t n);
int			ft_strcmp(const char *s1, const char *s2);
int			ft_atoi(const char *nptr);
char		*ft_strrchr(const char *s, int c);
char		*ft_strtrim(char const *s1, char const *set);


#endif
