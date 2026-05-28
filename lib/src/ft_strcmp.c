#include <stddef.h>
#include <stdint.h>

int	ft_strncmp(const char *s1, const char *s2, size_t n);
int	ft_strcmp(const char *s1, const char *s2) {
	return ft_strncmp(s1, s2, SIZE_MAX);
}

