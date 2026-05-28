#ifndef HASH_H
# define HASH_H

typedef struct s_nlist	t_nlist;
typedef struct s_nlist {
	char		*name; // key
	unsigned int	defn; // val
	t_nlist		*next;
}	t_nlist;

unsigned int	hash(const char *s);
t_nlist		*lookup(const char *s);
t_nlist		*registerTable(const char *name, unsigned int defn);
void		freeHashtab(void);

#endif
