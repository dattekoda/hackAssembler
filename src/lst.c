#include <stdlib.h>
#include "lst.h"

t_lst	*createLst(void *data, t_lst *next) {
	t_lst	*lst;

	lst = malloc(sizeof(t_lst));
	if (!lst)
		return NULL;
	lst->data = data;
	lst->next = next;
	return lst;
}

void	delLst(t_lst *lst, void (*del)(void*)) {
	del(lst->data);
	free(lst);
}

void	destructLst(t_lst *head, void (*del)(void*)) {
	t_lst	*tmp;

	while (head) {
		tmp = head->next;
		delLst(head, del);
		head = tmp;
	}
}

