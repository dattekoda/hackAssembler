#include <limits.h>
#include <stddef.h>

int	ft_isspace(int c);
int	ft_isnum(int c);
int	ft_strncmp(const char *s1, const char *s2, size_t n);

int	ft_atoi(const char *s) {
	int	isMinus;
	int	i;

	isMinus = 0;
	i = 0;
	while (ft_isspace(*s))
		s++;
	if (*s == '-' || *s == '+')
		isMinus = (*(s++) == '-');
	while (ft_isnum(*s))
		i = 10 * i + *(s++) - '0';
	return (1 - isMinus * 2) * i;
}

// #include <stdio.h>
// int	main(int argc, char *argv[]) {
// 	for (int i = 1; i < argc; i++)
// 		printf("%d\n", ft_atoi(argv[i]));
// 	return 0;
// }
