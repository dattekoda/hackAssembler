#include <stdio.h>
int	main(int argc, char *argv[]) {
	if (argc != 2)
		return 1;

	int	fd = open(argv[1]);
	char	*line;

	ft_get_next_line(&line, fd);
	printf("%s\n", line);
	free(line);
	ft_get_next_line(NULL, -7);
	return 0;
}

