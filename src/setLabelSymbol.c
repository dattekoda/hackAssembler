//TODO: set input like (var) to symbol table

#include "lib.h"
#include "lst.h"
#include "hash.h"
#include <string.h>
#include <errno.h>

void	error(const char *fmt, ...);
int	setLabelSymbol(char *line, int line_no);
t_nlist	*lookupPredefined(const char *key);

static int	isSkippableLine(const char *line);

int	constructSymbolTable(const t_lst *lst) {
	int	line_no = 0;

	for (; lst; lst = lst->next) {
		if (isSkippableLine(lst->data))
			continue ;
		char	*symbol_ptr = ft_strchr(lst->data, '(');
		if (symbol_ptr) {
			if (setLabelSymbol(symbol_ptr + 1, line_no))
				return 1;
			continue ;
		}
		++line_no;
	}
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

int	setLabelSymbol(char *symbol_ptr, int line_no) {
	char	*closePtr = ft_strchr(symbol_ptr, ')');

	if (!closePtr)
		return (error("set label symbol correctly"), 1);
	*closePtr = '\0';
	if (lookupPredefined(symbol_ptr))
		return (error("Don't use predefined symbol as a label symbol"), 1);
	if (!registerTable(symbol_ptr, line_no))
		return (error("%s\n", strerror(errno)), 1);
	return 0;
}

int	isPredefined(char *symbol) {
	(void)symbol;
	return 0;
}

