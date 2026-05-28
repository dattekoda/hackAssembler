#include "hash.h"
#include "lib.h"
#include <stdio.h>
#include <stdlib.h>

#define HASH_SIZE 101
// static const t_nlist	PreSymbolTable[HASH_SIZE];
#define PACKET_SIZE	8

static unsigned int	hash1(const char *key);
static unsigned int	hash2(const char *key, unsigned int d);

/*
@return: if not found NULL, else const pointer
*/
t_nlist	*lookupPredefined(const char *key) {
        static const unsigned int       hash1_table[8] = {8, 26, 351, 0, 1, 0, 0, 14};
        static const t_nlist    hash2_table[23] = {
                        {"R0", 0, NULL},
                        {"R12", 12, NULL},
                        {"ARG", 2, NULL},
                        {"SCREEN", 16384, NULL},
                        {"SP", 0, NULL},
                        {"R9", 9, NULL},
                        {"THIS", 3, NULL},
                        {"R11", 11, NULL},
                        {"LCL", 1, NULL},
                        {"R10", 10, NULL},
                        {"R15", 15, NULL},
                        {"R4", 4, NULL},
                        {"R5", 5, NULL},
                        {"R8", 8, NULL},
                        {"R2", 2, NULL},
                        {"R1", 1, NULL},
                        {"R3", 3, NULL},
                        {"THAT", 4, NULL},
                        {"R6", 6, NULL},
                        {"R14", 14, NULL},
                        {"R13", 13, NULL},
                        {"KEYBOARD", 24576, NULL},
                        {"R7", 7, NULL}
                };

	t_nlist	*tmp = (t_nlist *)&hash2_table[hash2(key, hash1_table[hash1(key)])];
	if (ft_strcmp(tmp->name, key))
		return NULL;
	return tmp;
}

static unsigned int	hash2(const char *key, unsigned int d) {
	unsigned int	h = d ^ 2166136261U;

	while (*key)
		h = (h ^ (unsigned char)*(key++)) * 16777619U;
	return h % 23;
}

static unsigned int	hash1(const char *key) {
	unsigned int	hashval;

	for (hashval = 0; *key; ++key)
		hashval = *key + (100003 ^ hashval);
	return hashval % PACKET_SIZE;
}

// typedef struct s_packet {
// 	int		size;
// 	unsigned int	d;
// 	unsigned int	hash1_val;
// 	t_nlist		*head;
// }	t_packet;
//
// void	addNlist(t_packet *p, const t_nlist *nl) {
// 	p->size++;
//
// 	t_nlist	*tmp = p->head;
// 	p->head = malloc(sizeof(t_nlist));
// 	ft_memmove(p->head, nl, sizeof(t_nlist));
// 	p->head->next = tmp;
// }
//
// static void	swap(void *p, int i, int j, size_t size) {
// 	unsigned char	*base;
// 	unsigned char	*a;
// 	unsigned char	*b;
// 	unsigned char	tmp;
//
// 	base = (unsigned char *)p;
// 	a = base + i * size;
// 	b = base + j * size;
// 	for (size_t k = 0; k < size; ++k) {
// 		tmp = a[k];
// 		a[k] = b[k];
// 		b[k] = tmp;
// 	}
// }
//
// static int	compar(const void *p1, const void *p2) {
// 	return (((const t_packet *)p2)->size - ((const t_packet *)p1)->size);
// }
//
// static void	req_qsort(unsigned char *base, int l, int r, size_t size, int (*compar)(const void *, const void *)) {
// 	int	i, last;
//
// 	if (l >= r)
// 		return ;
// 	swap(base, l, l + (r - l) / 2, size);
// 	last = l;
// 	for (i = l + 1; i <= r; ++i) {
// 		if (compar(base + i * size, base + l * size) < 0)
// 			swap(base, ++last, i, size);
// 	}
// 	swap(base, l, last, size);
// 	req_qsort(base, l, last - 1, size, compar);
// 	req_qsort(base, last + 1, r, size, compar);
// }
//
// static void	ft_qsort(void *base, size_t nmemb, size_t size,
// 	  int (*compar)(const void *, const void *)) {
// 	req_qsort((unsigned char *)base, 0, nmemb - 1, size, compar);
// }

/*
static void	printHashTables(void) {
	static const t_nlist	list[23] = {
		{"SP",0, NULL},
		{"LCL",1, NULL},
		{"ARG",2, NULL},
		{"THIS",3, NULL},
		{"THAT",4, NULL},
		{"R0",0, NULL},
		{"R1",1, NULL},
		{"R2",2, NULL},
		{"R3",3, NULL},
		{"R4",4, NULL},
		{"R5",5, NULL},
		{"R6",6, NULL},
		{"R7",7, NULL},
		{"R8",8, NULL},
		{"R9",9, NULL},
		{"R10",10, NULL},
		{"R11",11, NULL},
		{"R12",12, NULL},
		{"R13",13, NULL},
		{"R14",14, NULL},
		{"R15",15, NULL},
		{"SCREEN",16384, NULL},
		{"KEYBOARD",24576, NULL}
	};
	t_packet	each_size[PACKET_SIZE] = {0};
	for (int i = 0; i < PACKET_SIZE; ++i)
		each_size[i].hash1_val = i;
	for (size_t i = 0; i < sizeof(list) / sizeof(list[0]); ++i) {
		unsigned int	h1val = hash1(list[i].name);
		addNlist(&each_size[h1val], &list[i]);
	}
	ft_qsort(each_size, PACKET_SIZE, sizeof(t_packet), compar);
	t_nlist	chd_table[23] = {0};
	for (int pi = 0; pi < PACKET_SIZE; ++pi) {
		int	d = 0;
		for (d = 0;; ++d) {
			t_nlist	*curr = each_size[pi].head;
			char	alreadyExist[23] = {0};
			while (curr) {
				unsigned int	h2val = hash2(curr->name, d);
				if (alreadyExist[h2val] == 1 || chd_table[h2val].name)
					break ;
				alreadyExist[h2val] = 1;
				curr = curr->next;
			}
			if (!curr)
				break ;
		}
		t_nlist	*curr = each_size[pi].head;
		each_size[pi].d = d;
		while (curr) {
			int 	h2val = hash2(curr->name, d);
			chd_table[h2val] = *curr;
			curr = curr->next;
		}
	}
	unsigned int	dlist[PACKET_SIZE];
	for (int i = 0; i < PACKET_SIZE; ++i)
		dlist[each_size[i].hash1_val] = each_size[i].d;
	printf("\tstatic const unsigned int\thash1_table[%d] = {", PACKET_SIZE);
	for (int i = 0; i < PACKET_SIZE; ++i) {
		printf("%d", dlist[i]);
		if (i != PACKET_SIZE - 1)
			printf(", ");
	}
	printf("};\n");
	printf("\tstatic const t_nlist\thash2_table[%d] = {\n", 23);
	for (int i = 0; i < 23; ++i) {
		printf("\t\t\t{\"%s\", %u, NULL}", chd_table[i].name, chd_table[i].defn);
		if (i != 23 - 1)
			printf(",\n");
		else
			printf("\n");
	}
	printf("\t\t};\n");
}
*/
// int	main(int argc, char *argv[]) {
// 	if (argc == 1) {
// 		printHashTables();
// 		return 1;
// 	}
// 	for (int i = 1; i < argc; ++i) {
// 		t_nlist	*tmp = lookupPredefined(argv[i]);
// 		if (tmp)
// 			printf("%s: %u\n", tmp->name, tmp->defn);
// 		else
// 			printf("nothing found\n");
// 	}
// }

