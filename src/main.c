#include <stddef.h>
#include <stdarg.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <stdint.h>
#include "lst.h"

size_t	ft_strlen(const char *s);
int	ft_strncmp(const char *s1, const char *s2, size_t n);
int	ft_memmove(void *dst, const void *src, size_t n);
int	ft_isspace(int c);
char	*ft_strchr(const char *s, int c);
int	ft_get_next_line(char **line, int fd);

void		error(const char *fmt, ...);
static int	isInvalidArg(int argc, char *argv[]);
int		readFileToLst(t_lst **lst, const char *file_name);
int		assembleHack(const t_lst *lst);

int	main(int argc, char *argv[]) {
	t_lst	*lst;

	if (isInvalidArg(argc, argv))
		return (error("%s prog.asm", argv[0]), 1);
	if (readFileToLst(&lst, argv[1]))
		return 1;
	if (assembleHack(lst))
		return 1;
	destructLst(lst, free);
	return 0;
}

typedef enum {
	A_INSTRUCTION, //@123
	C_INSTRUCTION, // dest=comp; jmp
	L_INSTRUCTION, // (LOOP)
	N_INSTRUCTION // INVALID
}	e_instructionType;

//@100
//D=D+1;JMP
typedef struct s_info {
	e_instructionType	i;
	uint16_t	labelSymbol;
	// char		*variableSymbol;
	char		*dest;
	char		*comp;
	char		*jmp;
}	t_info;

static int	isSkippableLine(const char *line);
static int	constructInfo(t_info **info, const char *line);

int	parseHack(t_lst **info_lst, const t_lst *lst) {
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

static int	isSkippableLine(const char *line) {
	while (ft_isspace(*line))
		line++;
	if (*line == '\0')
		return 1;
	if (ft_strncmp(line, "//", 2) == 0)
		return 1;
	return 0;
}

static e_instructionType	getInstruction(const char *line);
static int	constructInfo(t_info **info, const char *line) {
	t_info	_info;

	while (ft_isspace(*line))
		line++;
	_info.i = getInstruction(line);
	if (_info.i == A_INSTRUCTION)
		printf("A\n");
	// 	setAinfo(&_info, line);
	else if (_info.i == C_INSTRUCTION)
		printf("C\n");
	// 	setCinfo(&_info, line);
	else if (_info.i == L_INSTRUCTION)
		printf("L\n");
	// 	setLinfo(&_info, line);
	*info = malloc(sizeof(t_info));
	if (!*info)
		return 1;
	ft_memmove(*info, &_info, sizeof(t_info));
	return 0;
}

static e_instructionType	getInstruction(const char *line) {
	if (*line == '@')
		return A_INSTRUCTION;
	else if (ft_strchr(line, '('))
		return L_INSTRUCTION;
	return C_INSTRUCTION;
}

// int	codeHack(const t_lst *data) {
// }

int	assembleHack(const t_lst *lst) {
	t_lst	*info_lst;

	if (parseHack(&info_lst, lst))
		return 1;
	// if (codeHack(data))
	// 	return 1;
	return 0;
}

// static int	hasMoreLines(const t_lst *lst) {
// 	return lst != NULL;
// }

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

