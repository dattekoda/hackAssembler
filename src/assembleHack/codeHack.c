#include "lst.h"
#include "lib.h"
#include "info.h"
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>

extern char	*g_file_name;
extern size_t	g_file_name_len;

void	error(const char *fmt, ...);

static int	creatHackFile(void);
static void	writeHackLines(int fd, const t_lst *info_lst);
int	codeHack(const t_lst *info_lst) {
	int	fd;

	fd = creatHackFile(); 
	if (fd < 0)
		return 1;
	writeHackLines(fd, info_lst);
	close(fd);
	return 0;
}

static int	creatHackFile(void) {
	char	*hack_file_name;
	int	fd;

	hack_file_name = malloc(g_file_name_len + 2);
	if (!hack_file_name)
		return (error("%s", strerror(errno)), 1);
	ft_memmove(hack_file_name, g_file_name, g_file_name_len - 4);
	ft_memmove(hack_file_name + g_file_name_len - 4, ".hack", 6);
	fd = open(hack_file_name, O_CREAT | O_WRONLY | O_TRUNC, 0775);
	if (fd < 0)
		error("%s: %s", hack_file_name, strerror(errno));
	free(hack_file_name);
	return fd;
}

static void	writeHackLines(int fd, const t_lst *info_lst) {
	char	buf[17];

	buf[16] = '\n';
	while (info_lst) {
		if (((t_info *)info_lst->data)->i == L_INSTRUCTION)
			info_lst = info_lst->next;
		setHackLine(buf, info_lst->data);
		write(fd, buf, 17);
		info_lst = info_lst->next;
	}
}

