#include <stdio.h>
int	main(int argc, char *argv[]) {
	for (int i = 2; i < argc; i++)
		printf("%s\n", ft_strchr(argv[i], argv[1][0]));
	return 0;
}

