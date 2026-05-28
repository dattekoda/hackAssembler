#include <stdarg.h>
#include <unistd.h>
#include <stdio.h>

void	error(const char *fmt, ...) {
	va_list	ap;

	dprintf(STDERR_FILENO, "hackAssembler Error: ");
	va_start(ap, fmt);
	vdprintf(STDERR_FILENO, fmt, ap);
	dprintf(STDERR_FILENO, "\n");
	va_end(ap);
}

