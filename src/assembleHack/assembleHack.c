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

