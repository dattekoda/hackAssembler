#include "lib.h"
#include "info.h"
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <stdio.h>

void	error(const char *fmt, ...);

int				setInfo(t_info *info, char *line);
static e_instructionType	getInstruction(const char *line);
static void			setAinfo(t_info *_info, const char *line);
static void			setCinfo(t_info *_info, char *line);
static int			isInvalidInfo(const t_info *_info);

int	setInfo(t_info *info, char *line) {
	ft_memset(info, 0, sizeof(t_info));
	while (ft_isspace(*line))
		line++;
	info->i = getInstruction(line);
	if (info->i == A_INSTRUCTION)
		setAinfo(info, line);
	else if (info->i == C_INSTRUCTION)
		setCinfo(info, line);
	else if (info->i == L_INSTRUCTION)
		printf("L\n");
	// 	setLinfo(&_info, line);
	if (isInvalidInfo(info))
		return 1;
	return 0;
}

// wanna identify which line is invalid
static int	isInvalidInfo(const t_info *_info) {
	if (_info->i == C_INSTRUCTION) {
		if (!*_info->comp)
			return (error("set comp"), 1);
	}
	return 0;
}

// handle A instruction like @432
// TODO: need to handle symbol pattern like @loop
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
		_info->dest = line;
		_info->comp = equal_p + 1;
	} else {
		line = _info->comp = line;
	}
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

