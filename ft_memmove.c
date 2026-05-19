#include <stddef.h>

void	*ft_memmove(void *dst, const void *src, size_t n) {
	unsigned char	*_dst = (unsigned char *)dst;
	unsigned char	*_src = (unsigned char *)src;

	if (_dst < _src) {
		while (n) {
			*(_dst++) = *(_src++);
			--n;
		}
		return dst;
	}
	while (n) {
		--n;
		*(_dst + n) = *(_src + n);
	}
	return dst;
}

// #include <unistd.h>
// int	main(void) {
// 	char	buf[1024] = {0};
//
// 	ft_memmove(buf, "0123456789abcdef", 16);
// 	ft_memmove(buf + 10, buf, 16);
// 	ft_memmove(buf + 5, buf+10, 16);
// 	write(1, buf, 26);
// }
//
