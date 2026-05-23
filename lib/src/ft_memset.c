#include <stddef.h>

void	*ft_memset(void *s, int c, size_t n) {
	unsigned char	*ucs;

	ucs = (unsigned char *)s;
	while (n) {
		*(ucs++) = c;
		n--;
	}
	return s;
}

