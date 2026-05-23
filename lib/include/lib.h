#ifndef LIB_H
# define LIB_H

# include <stddef.h>

int	ft_atoi(const char *s);

int	ft_get_next_line(char **line, int fd);

int	ft_isnum(int c);
int	ft_isspace(int c);

void	*ft_memmove(void *dst, const void *src, size_t n);
void	*ft_memset(void *s, int c, size_t n);

char	*ft_strchr(const char *s, int c);
char	*ft_strdup(const char *s);
char	*ft_strjoin(const char *s1, const char *s2);
size_t	ft_strlen(const char *s);
int	ft_strncmp(const char *s1, const char *s2, size_t n);
char	*ft_strndup(const char *s, size_t n);

#endif
