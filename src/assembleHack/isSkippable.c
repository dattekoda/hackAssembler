#include "lib.h"

int	isSkippableLine(const char *line) {
	while (ft_isspace(*line))
		line++;
	if (*line == '\0')
		return 1;
	if (ft_strncmp(line, "//", 2) == 0)
		return 1;
	return 0;
}

