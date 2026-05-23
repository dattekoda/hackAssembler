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
	char		*jump;
}	t_info;

static int	isSkippableLine(const char *line);
static int	constructInfo(t_info **info, char *line);

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
static void			setAinfo(t_info *_info, const char *line);
static void			setCinfo(t_info *_info, char *line);

static int	constructInfo(t_info **info, char *line) {
	t_info	_info;

	ft_memset(&_info, 0, sizeof(t_info));
	while (ft_isspace(*line))
		line++;
	_info.i = getInstruction(line);
	if (_info.i == A_INSTRUCTION)
		setAinfo(&_info, line);
	else if (_info.i == C_INSTRUCTION)
		setCinfo(&_info, line);
	else if (_info.i == L_INSTRUCTION)
		printf("L\n");
	// 	setLinfo(&_info, line);
	*info = malloc(sizeof(t_info));
	if (!*info)
		return 1;
	ft_memmove(*info, &_info, sizeof(t_info));
	return 0;
}

// handle A instruction like @432
// TODO: need to modify to handle symbol pattern like @loop
static void	setAinfo(t_info *_info, const char *line) {
	_info->labelSymbol = ft_atoi(line + 1) & (0x00007FFF);
	// skip "@" so line + 1
	// printf("A: %d\n", _info->labelSymbol);
}

static void	setCinfo(t_info *_info, char *line) {
	char	*semi_column_p = ft_strchr(line, ';');
	char	*equal_p = ft_strchr(line, '=');

	if (semi_column_p) {
		*semi_column_p = '\0';
		_info->jump = semi_column_p + 1;
	}
	if (equal_p) {
		*equal_p = '\0';
		_info->dest = equal_p + 1;
	}
	_info->comp = line;
	// printf("C: comp: %s, dest: %s, jump: %s\n",
	// 		_info->comp, _info->dest, _info->jump);
}

//(loop) = 10
//@loop = 10
//@r1 = 1
// static char	*getSymbol(char *line) {
// 	char	*closep = ft_strchr(line + 1, ')');
//
// 	if (closep) {
// 		*closep = '\0';
// 		return str;
// 	}
// }

static e_instructionType	getInstruction(const char *line) {
	if (*line == '@')
		return A_INSTRUCTION;
	else if (ft_strchr(line, '('))
		return L_INSTRUCTION;
	return C_INSTRUCTION;
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

static void	setHackLine(char buf[16], const t_info *info);
static void	writeHackLines(int fd, const t_lst *info_lst) {
	char	buf[17];

	buf[16] = '\n';
	while (info_lst) {
		setHackLine(buf, info_lst->data);
		write(fd, buf, 17);
		info_lst = info_lst->next;
	}
}

static void	setALine(char buf[16], const t_info *info);
static void	setCLine(char buf[16], const t_info *info);

static void	setHackLine(char buf[16], const t_info *info) {
	if (info->i == A_INSTRUCTION)
		setALine(buf, info);
	else if (info->i == C_INSTRUCTION)
		setCLine(buf, info);
	else if (info->i == L_INSTRUCTION)
		ft_memset(buf, '1', sizeof(char) * 16);
}

static void	setALine(char buf[16], const t_info *info) {
	uint16_t	offset = 1 << 14;
	unsigned char	idx = 0;

	buf[0] = '0';
	while (++idx < 16) {
		if (offset & info->labelSymbol)
			buf[idx] = '1';
		else
			buf[idx] = '0';
		offset >>= 1;
	}
}

static void	setComp(char buf[16], const t_info *info);
static void	setDest(char buf[16], const t_info *info);
static void	setJump(char buf[16], const t_info *info);

static void	setCLine(char buf[16], const t_info *info) {
	(void)info;
	ft_memset(buf, '1', sizeof(char) * 3);
	ft_memset(buf + 3, '0', sizeof(char) * 13);
	setComp(buf, info);
	setDest(buf, info);
	setJump(buf, info);
}

static void	setComp(char buf[16], const t_info *info) {
	char	*m_ptr = ft_strchr(buf, 'M');

	(void)info;
	if (m_ptr) {
		*m_ptr = 'A';
		buf[3] = '1';
	}
}

static void	setDest(char buf[16], const t_info *info) {
	if (!info->dest)
		return ;
	if (ft_strchr(info->dest, 'M'))
		buf[12] = '1';
	if (ft_strchr(info->dest, 'D'))
		buf[11] = '1';
	if (ft_strchr(info->dest, 'M'))
		buf[10] = '1';
}

static char	*getJumpBinary(const char *jump) {
	(void)jump;
	return ("000");
}

static void	setJump(char buf[16], const t_info *info) {
	if (!info->jump)
		return ;
	ft_memmove(buf + 13, getJumpBinary(info->jump), 3);
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
	g_file_name_len = ft_strlen(argv[1]);

	if (g_file_name_len < 5
			|| ft_strncmp(argv[1] + g_file_name_len - 4, ".asm", 4))
		return 1;
	return 0;
}

