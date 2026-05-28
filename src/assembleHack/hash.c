#include "hash.h"
#include "lib.h"
#include <stddef.h>
#include <stdlib.h>

#define	HASHSIZE	101
static t_nlist	*hashtab[HASHSIZE];

t_nlist	*lookupPredefined(const char *key);

// hash: form hash value for string s
unsigned int	hash(const char *s) {
	unsigned int	hashval;

	for (hashval = 0; *s; s++)
		hashval = hashval * 71+ *s;
	return hashval % HASHSIZE;
}

t_nlist	*lookup(const char *s) {
	t_nlist	*np;

	np = lookupPredefined(s);
	if (np)
		return np;
	for (np = hashtab[hash(s)]; np; np = np->next) {
		if (ft_strcmp(s, np->name) == 0)
			return np;
	}
	return NULL;
}

t_nlist	*registerTable(const char *name, unsigned int defn) {
	t_nlist		*np;

	if ((np = lookup(name)) == NULL) {
		np = (t_nlist*)malloc(sizeof(*np));
		if (!np || (np->name = ft_strdup(name)) == NULL)
			return NULL;
		unsigned int	hashval = hash(name);
		np->next = hashtab[hashval];
		hashtab[hashval] = np;
	}
	np->defn = defn;
	return np;
}

static void	freeNlist(t_nlist *np) {
	while (np) {
		t_nlist	*tmp = np->next;
		free(np->name);
		free(np);
		np = tmp;
	}
}

void	freeHashtab(void) {
	for (int i = 0; i < HASHSIZE; ++i) {
		freeNlist(hashtab[i]);
	}
}

// #include <stdio.h>
// int	main(int argc, char  *argv[]) {
//
// 	if (argc == 1 || argc == 2)
// 		return 1;
// 	for (int i = 1; i < argc - 1; ++i) {
//		registerTable(argv[i], i);
// 	}
// 	t_nlist	*nl = lookup(argv[argc - 1]);
// 	if (nl)
// 		printf("%u\n", nl->defn);
// 	freeHashtab();
// }
//
