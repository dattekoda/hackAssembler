#include "hash.h"
#include "info.h"
#include "lst.h"
#include <errno.h>
#include <fcntl.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int ft_atoi(const char *s);
size_t ft_strlen(const char *s);
int ft_strncmp(const char *s1, const char *s2, size_t n);
int ft_memmove(void *dst, const void *src, size_t n);
void *ft_memset(void *s, int c, size_t n);
int ft_isspace(int c);
char *ft_strchr(const char *s, int c);
int ft_get_next_line(char **line, int fd);

int isInvalidArg(int argc, char *argv[]);
void error(const char *fmt, ...);
int readFileToLst(t_lst **lst);
int assembleHack(const t_lst *lst);

size_t g_file_name_len;
char *g_file_name;

int main(int argc, char *argv[]) {
  t_lst *lst;

  if (isInvalidArg(argc, argv))
    return (error("%s prog.asm", argv[0]), 1);
  g_file_name = argv[1];
  if (readFileToLst(&lst))
    return 1;
  if (assembleHack(lst))
    return 1;
  destructLst(lst, free);
  return 0;
}
