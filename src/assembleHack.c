#include "lib.h"
#include "hash.h"
#include "lst.h"
#include "info.h"
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

extern char	*g_file_name;
extern size_t	g_file_name_len;
void	error(const char *fmt, ...);

int	parseHack(t_lst **info_lst, const t_lst *lst);
int	codeHack(const t_lst *info_lst);

int	assembleHack(const t_lst *lst) {
	t_lst	*info_lst;

	if (parseHack(&info_lst, lst))
		return 1;
	if (codeHack(info_lst))
		return 1;
	destructLst(info_lst, free);
	return 0;
}

// TODO: implement constructSymbolTable()
// int	constructSymbolTable(const t_lst *lst) {
// }
int	constructSymbolTable(const t_lst *lst);
int	constructInfos(t_lst **info_lst, const t_lst *lst);
int	parseHack(t_lst **info_lst, const t_lst *lst) {
	if (constructSymbolTable(lst))
		return (freeHashtab(), 1);
	if (constructInfos(info_lst, lst))
		return (freeHashtab(), 1);
	freeHashtab();
	return 0;
}

static int	isSkippableLine(const char *line);
static int	constructInfo(t_info **info, char *line);

int	constructInfos(t_lst **info_lst, const t_lst *lst) {
	t_lst	head;
	t_lst	*curr;
	t_info	*info;

	curr = &head;
	while (curr && lst) {
		if (isSkippableLine(lst->data)) {
			lst = lst->next;
			continue ;
		}
		if (constructInfo(&info, lst->data))
			return (destructLst(head.next, free), 1);
		curr->next = createLst(info, NULL);
		curr = curr->next;
		lst = lst->next;
	}
	if (!curr)
		return (destructLst(head.next, free), 1);
	*info_lst = head.next;
	return 0;
}

static int	constructInfo(t_info **info, char *line) {
	*info = malloc(sizeof(t_info));
	if (!*info)
		return (error("%s", strerror(errno)), 1);
	if (setInfo(*info, line))
		return (free(*info), 1);
	return 0;
}

static int	isSkippableLine(const char *line) {
	while (ft_isspace(*line))
		line++;
	if (*line == '\0')
		return 1;
	if (ft_strncmp(line, "//", 2) == 0)
		return 1;
	return 0;
}

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

