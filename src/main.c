#include "lst.h"
#include "hash.h"
#include "info.h"
#include <stddef.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <stdint.h>

int	ft_atoi(const char *s);
size_t	ft_strlen(const char *s);
int	ft_strncmp(const char *s1, const char *s2, size_t n);
int	ft_memmove(void *dst, const void *src, size_t n);
void	*ft_memset(void *s, int c, size_t n);
int	ft_isspace(int c);
char	*ft_strchr(const char *s, int c);
int	ft_get_next_line(char **line, int fd);

void		error(const char *fmt, ...);
static int	isInvalidArg(int argc, char *argv[]);
int		readFileToLst(t_lst **lst);
int		assembleHack(const t_lst *lst);

size_t	g_file_name_len;
char	*g_file_name;

int	main(int argc, char *argv[]) {
	t_lst	*lst;

	if (isInvalidArg(argc, argv))
		return (error("%s prog.asm", argv[0]), 1);
	g_file_name = argv[1];
	if (readFileToLst(&lst))
		return 1;
	if (assembleHack(lst))
		return 1;
	destructLst(lst, free);
	return 0;
}

// TODO: implement constructSymbolTable()
// int	constructSymbolTable(const t_lst *lst) {
// }
int	constructSymbolTable(const t_lst *lst);
int	constructInfos(t_lst **info_lst, const t_lst *lst);
int	parseHack(t_lst **info_lst, const t_lst *lst) {
	// if (constructSymbolTable(lst))
	// 	return (freeHashtab(), 1);
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
		setHackLine(buf, info_lst->data);
		write(fd, buf, 17);
		info_lst = info_lst->next;
	}
}

int	assembleHack(const t_lst *lst) {
	t_lst	*info_lst;

	if (parseHack(&info_lst, lst))
		return 1;
	if (codeHack(info_lst))
		return 1;
	destructLst(info_lst, free);
	return 0;
}

// static int	hasMoreLines(const t_lst *lst) {
// 	return lst != NULL;
// }

static int	allocLst(t_lst **lst, int fd);
int	readFileToLst(t_lst **lst) {
	int	fd = open(g_file_name, O_RDONLY);

	if (fd < 0)
		return (error("%s: %s", g_file_name, strerror(errno)), 1);
	if (allocLst(lst, fd))
		return (close(fd), error("%s", strerror(errno)), 1);
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

static int	isInvalidArg(int argc, char *argv[]) {
	if (argc != 2)
		return 1;
	g_file_name_len = ft_strlen(argv[1]);

	if (g_file_name_len < 5
			|| ft_strncmp(argv[1] + g_file_name_len - 4, ".asm", 4))
		return 1;
	return 0;
}

