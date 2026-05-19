#include <stddef.h>

size_t	ft_strlen(const char *s) {
	const char	*bgn = s;

	while (*s)
		s++;
	return s - bgn;
}

