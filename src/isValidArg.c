#include "lib.h"
extern size_t	g_file_name_len;
extern char	*g_file_name;

int	isInvalidArg(int argc, char *argv[]) {
	if (argc != 2)
		return 1;
	g_file_name_len = ft_strlen(argv[1]);
	if (g_file_name_len < 5
			|| ft_strncmp(argv[1] + g_file_name_len - 4, ".asm", 4))
		return 1;
	return 0;
}

