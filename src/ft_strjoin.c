#include <stdlib.h>
#include <stddef.h>

size_t	ft_strlen(const char *s);
void	*ft_memmove(void *dst, const void *src, size_t n);

char	*ft_strjoin(const char *s1, const char *s2) {
	char	*join;
	const size_t	s1_len = ft_strlen(s1);
	const size_t	s2_len = ft_strlen(s2);

	join = malloc(s1_len + s2_len + 1);
	if (!join)
		return NULL;
	ft_memmove(join + s1_len, s2, s2_len + 1);
	return ft_memmove(join, s1, s1_len);
}

