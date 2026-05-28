#include "lst.h"
#include "lib.h"
#include "info.h"
#include "hash.h"
#include <stdlib.h>
#include <string.h>
#include <errno.h>

void	error(const char *fmt, ...);
int	isSkippableLine(const char *line);

int	constructSymbolTable(const t_lst *lst);

static int	constructInfo(t_info **info, char *line);
static int	constructInfos(t_lst **info_lst, const t_lst *lst);

int	parseHack(t_lst **info_lst, const t_lst *lst) {
	if (constructSymbolTable(lst))
		return (freeHashtab(), 1);
	if (constructInfos(info_lst, lst))
		return (freeHashtab(), 1);
	freeHashtab();
	return 0;
}

static int	constructInfos(t_lst **info_lst, const t_lst *lst) {
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

