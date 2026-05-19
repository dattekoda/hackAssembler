#include <stdio.h>
int main(int argc, char *argv[]) {
	if (argc == 1)
		return 1;
	char	*s = ft_strdup(argv[1]);
	printf("%s\n", s);
	free(s);
	return 0;
}

