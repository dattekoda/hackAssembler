char	*ft_strchr(const char *s, int c) {
	while (*s) {
		if (*s == c)
			return (char *)s;
		s++;
	}
	return ((void *)0);
}

