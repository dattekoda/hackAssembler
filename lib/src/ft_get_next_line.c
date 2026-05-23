#include <stdlib.h> // malloc()
#include <unistd.h> // read()

#ifndef NULL
# define NULL ((void *0))
#endif

#ifndef BUFFER_SIZE
# define BUFFER_SIZE 4096
#endif

#ifndef INCLUDE_NL
# define INCLUDE_NL 0
#endif

char	*ft_strdup(const char *s);
char	*ft_strndup(const char *s, size_t n);
char	*ft_strjoin(const char *s1, const char *s2);
char	*ft_strchr(const char *s, int n);

static void	safeFree(char **ptr);
static ssize_t	fdToStore(int fd, char **store);
static int	storeToLine(char **line, const char *store, const char *end_line);
static int	updateStore(char **store, const char *end_line);

int	ft_get_next_line(char **line, int fd) {
	static char	*store = NULL;
	ssize_t		read_size;
	char		*end_line;

	if (fd == -7 && store)
		return (safeFree(&store), 0);
	if (fd < 0 || BUFFER_SIZE <= 0 || !line || INCLUDE_NL < 0 || 1 < INCLUDE_NL)
		return -1;
	read_size = fdToStore(fd, &store);
	if (read_size < 0)
		return -1;
	if (read_size == 0 && !store[0])
		return (safeFree(&store), 0);
	end_line = ft_strchr(store, '\n');
	if (storeToLine(line, store, end_line))
		return (safeFree(&store), -1);
	if (updateStore(&store, end_line))
		return (free(*line), -1);
	return 1;
}

static void	safeFree(char **ptr) {
	free(*ptr);
	*ptr = NULL;
}

static ssize_t	fdToStore(int fd, char **store) {
	char	buf[BUFFER_SIZE+1];
	char	*tmp;
	ssize_t	read_ret;
	ssize_t	read_sum;

	if (!*store)
		*store = ft_strdup("");
	if (!*store)
		return -1;
	read_sum = 0;
	while (!ft_strchr(*store, '\n')) {
		read_ret = read(fd, buf, BUFFER_SIZE);
		if (read_ret < 0)
			return (safeFree(store), -1);
		if (read_ret == 0)
			break ;
		read_sum += read_ret;
		buf[read_ret] = '\0';
		tmp = *store;
		*store = ft_strjoin(*store, buf);
		free(tmp);
		if (!*store)
			return -1;
	}
	return read_sum;
}

static int	storeToLine(char **line, const char *store, const char *end_line) {
	if (!end_line)
		*line = ft_strdup(store);
	else
		*line = ft_strndup(store, end_line - store + INCLUDE_NL);
	if (!*line)
		return 1;
	return 0;
}

static int	updateStore(char **store, const char *end_line) {
	char	*tmp;

	if (end_line) {
		tmp = *store;
		*store = ft_strdup(end_line + 1);
		free(tmp);
		if (!*store)
			return 1;
		return 0;
	}
	safeFree(store);
	return 0;
}

// #include <fcntl.h>
// #include <stdio.h>
// int	main(int argc, char *argv[]) {
// 	if (argc != 2)
// 		return 1;
//
// 	int	fd = open(argv[1], O_RDONLY);
// 	char	*line;
//
// 	int	gnl_ret;
// 	while (1) {
// 		gnl_ret = ft_get_next_line(&line, fd);
// 		if (gnl_ret == -1)
// 			perror("gnl");
// 		if (gnl_ret == 0)
// 			break ;
// 		printf("%s", line);
// 		free(line);
// 	}
// 	ft_get_next_line(NULL, -7);
// 	return 0;
// }
