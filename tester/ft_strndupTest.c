#include <stdio.h>
int main(int argc, char *argv[]) {
	if (argc != 3)
		return 1;
	char	*s = ft_strndup(argv[1], atoi(argv[2]));
	printf("%s\n", s);
	free(s);
	return 0;
}

