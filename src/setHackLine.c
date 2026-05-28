#include <stdint.h>
#include "info.h"
#include "lib.h"

static void	setALine(char buf[16], const t_info *info);
static void	setCLine(char buf[16], const t_info *info);

void	setHackLine(char buf[16], const t_info *info) {
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

static void	setComp(char buf[7], const char *comp);
static void	setDest(char buf[3], const char *dest);
static void	setJump(char buf[3], const char *jump);

static void	setCLine(char buf[16], const t_info *info) {
	ft_memset(buf, '1', sizeof(char) * 3);
	ft_memset(buf + 3, '0', sizeof(char) * 13);
	setComp(buf + 3, info->comp);
	setDest(buf + 10, info->dest);
	setJump(buf + 13, info->jump);
}

static void	set1Comp(char buf[6], const char *comp);
static void	set2Comp(char buf[6], const char *comp);
static void	set3Comp(char buf[6], const char *comp);
static void	setComp(char buf[7], const char *comp) {
	const size_t	comp_len = ft_strlen(comp);
	char		*m_ptr = ft_strchr(comp, 'M');

	if (m_ptr) {
		*m_ptr = 'A';
		buf[0] = '1';
	}
	// printf("comp: %s\n", comp);
	if (comp_len == 1)
		set1Comp(buf + 1, comp);
	else if (comp_len == 2)
		set2Comp(buf + 1, comp);
	else if (comp_len == 3)
		set3Comp(buf + 1, comp);
}

static void	set1Comp(char buf[6], const char *comp) {
	if (*comp == '0')
		ft_memmove(buf, "10101", 5);
	else if (*comp == '1')
		ft_memset(buf, '1', 6);
	else if (*comp == 'D')
		ft_memset(buf + 2, '1', 2);
	else if (*comp == 'A')
		ft_memset(buf, '1', 2);
}

static void	set2Comp(char buf[6], const char *comp) {
	if (*comp == '-')
		buf[4] = '1';
	if (*(comp + 1) == 'A') {
		buf[5] = '1';
		ft_memset(buf, '1', 2);
	} else if (*(comp + 1) == 'D') {
		buf[5] = '1';
		ft_memset(buf + 2, '1', 2);
	} else {
		ft_memset(buf, '1', 3);
	}
}

static char	*get3CompBinary(const char *comp);
static void	set3Comp(char buf[6], const char *comp) {
	ft_memmove(buf, get3CompBinary(comp), 6);
}

static char	*get3CompBinary(const char *comp) {
	static const char	dic[10][9] \
		= {"D+1011111", "A+1110111", "D-1001110", \
		"A-1110010", "D+A000010", "D-A010011", \
		"A-D000111", "D&A000000", "D|A010101" };

	for (int i = 0; i < 9; ++i) {
		if (ft_strncmp(comp, dic[i], 3) == 0) {
			return (char *)dic[i] + 3;
		}
	}
	return "000000";
}

static void	setDest(char buf[3], const char *dest) {
	if (!dest)
		return ;
	if (ft_strchr(dest, 'M'))
		buf[2] = '1';
	if (ft_strchr(dest, 'D'))
		buf[1] = '1';
	if (ft_strchr(dest, 'A'))
		buf[0] = '1';
}

#ifndef JUMP_KEYTYPE_SIZE
# define JUMP_KEYTYPE_SIZE	7
#endif
static char	*getJumpBinary(const char *jump) {
	static const char	dic[7][JUMP_KEYTYPE_SIZE]
		= {"JGT001", "JEQ010", "JGE011", \
			"JLT100", "JNE101", "JLE110", "JMP111"};
	int	i = -1;

	while (++i < JUMP_KEYTYPE_SIZE) {
		if (ft_strncmp(jump, dic[i], 3) == 0)
			return (char *)dic[i] + 3;
	}
	return ("000");
}

static void	setJump(char buf[3], const char *jump) {
	if (!jump || ft_strlen(jump) != 3)
		return ;
	ft_memmove(buf, getJumpBinary(jump), 3);
}

