#ifndef INFO_H
# define INFO_H

# include <stdint.h>

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

int	setInfo(t_info *info, char *line);
void	setHackLine(char buf[16], const t_info *info);

#endif

