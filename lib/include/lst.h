#ifndef LST_H
# define LST_H

typedef struct s_lst	t_lst;

typedef struct s_lst {
	void	*data;
	t_lst	*next;
}	t_lst;

t_lst	*createLst(void *data, t_lst *next);

void	delLst(t_lst *lst, void (*del)(void*));
void	destructLst(t_lst *head, void (*del)(void*));

#endif
