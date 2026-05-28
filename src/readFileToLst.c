#include "lib.h"
#include "lst.h"
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>

extern size_t	g_file_name_len;
extern char	*g_file_name;
void	error(const char *fmt, ...);

static int	allocLst(t_lst **lst, int fd);
int	readFileToLst(t_lst **lst) {
	int	fd = open(g_file_name, O_RDONLY);

	if (fd < 0)
		return (error("%s: %s", g_file_name, strerror(errno)), 1);
	if (allocLst(lst, fd))
		return (close(fd), ft_get_next_line(NULL, -7), error("%s", strerror(errno)), 1);
	ft_get_next_line(NULL, -7);
	close(fd);
	return 0;
}

static int	allocLst(t_lst **lst, int fd) {
	int	gnl_ret;
	char	*line;
	t_lst	head;
	t_lst	*curr;

	curr = &head;
	while (curr) {
		gnl_ret = ft_get_next_line(&line, fd);
		if (gnl_ret == -1)
			return (destructLst(head.next, free), 1);
		if (gnl_ret == 0)
			break ;
		curr->next = createLst(line, NULL);
		curr = curr->next;
	}
	if (!curr)
		return (destructLst(head.next, free), 1);
	*lst = head.next;
	return 0;
}

