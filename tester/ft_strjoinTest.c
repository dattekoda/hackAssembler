#include <stdio.h>
int	main(int argc, char *argv[]) {
	if (argc != 3)
		return 1;

	char	*join = ft_strjoin(argv[1], argv[2]);
	printf("%s\n", join);
	free(join);
	return 0;
}

