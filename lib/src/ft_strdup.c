#include <stdlib.h>
#include <stddef.h>

size_t	ft_strlen(const char *s);
void	*ft_memmove(void *dst, const void *src, size_t n);

char	*ft_strdup(const char *s) {
	char		*dup;
	const size_t	s_len = ft_strlen(s);

	dup = malloc(s_len + 1);
	if (!dup)
		return NULL;
	dup[s_len] = 0;
	return ft_memmove(dup, s, s_len);
}

