#include <stddef.h>
#include <stdarg.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include "lst.h"

size_t	ft_strlen(const char *s);
int	ft_strncmp(const char *s1, const char *s2, size_t n);
int	ft_get_next_line(char **line, int fd);

void		error(const char *fmt, ...);
static int	isInvalidArg(int argc, char *argv[]);
int		readFileToLst(t_lst **lst, const char *file_name);

int	main(int argc, char *argv[]) {
	t_lst	*lst;

	if (isInvalidArg(argc, argv))
		return (error("%s prog.asm", argv[0]), 1);
	if (readFileToLst(&lst, argv[1]))
		return 1;
	// if (compileData(&lst))
	// 	return 1;
	destructLst(lst, free);
	return 0;
}

static int	allocLst(t_lst **lst, int fd);
int	readFileToLst(t_lst **lst, const char *file_name) {
	int	fd = open(file_name, O_RDONLY);

	if (fd < 0)
		return (error("%s: %s", file_name, strerror(errno)), 1);
	if (allocLst(lst, fd))
		return (close(fd), 1);
	close(fd);
	return 0;
}

int	allocLst(t_lst **lst, int fd) {
	int	gnl_ret;
	char	*line;
	t_lst	head;
	t_lst	*curr;

	curr = &head;
	while (curr) {
		gnl_ret = ft_get_next_line(&line, fd);
		if (gnl_ret == -1)
			return (destructLst(head.next, free), error("%s", strerror(errno)), 1);
		if (gnl_ret == 0)
			break ;
		curr->next = createLst(line, NULL);
		curr = curr->next;
	}
	if (!curr)
		return (destructLst(head.next, free), error("%s", strerror(errno)), 1);
	*lst = head.next;
	return 0;
}

void	error(const char *fmt, ...) {
	va_list	ap;

	dprintf(STDERR_FILENO, "hackAssembler Error: ");
	va_start(ap, fmt);
	vdprintf(STDERR_FILENO, fmt, ap);
	dprintf(STDERR_FILENO, "\n");
	va_end(ap);
}

static int	isInvalidArg(int argc, char *argv[]) {
	if (argc != 2)
		return 1;
	const size_t	file_name_len = ft_strlen(argv[1]);
	if (file_name_len < 5 || ft_strncmp(argv[1] + file_name_len - 4, ".asm", 4))
		return 1;
	return 0;
}

