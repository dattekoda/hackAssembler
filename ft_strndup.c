#include <stdlib.h>

size_t	ft_strlen(const char *s);
void	*ft_memmove(void *dst, const void *src, size_t n);

char	*ft_strndup(const char *s, size_t n) {
	char		*ndup;
	const size_t	s_len = ft_strlen(s);

	if (s_len < n)
		n = s_len;
	ndup = malloc(n + 1);
	if (!ndup)
		return NULL;
	ndup[n] = '\0';
	return ft_memmove(ndup, s, n);
}

