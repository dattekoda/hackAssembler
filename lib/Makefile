NAME	:=	lib.a
CC	:=	cc
CFLAGS	:=	-Wall -Wextra -Werror

AR	:=	ar rcs
# r: Insert the files into the archive (replacing older ones if they exist).
# c: Create the archive without showing a warning if it didn't exist before.
# s: Create an index (ranlib) inside the archive, which speeds up linking later.

INCDIR	:=	include

SRCDIR		:=	src
SRCFILES	:=	ft_atoi \
			ft_get_next_line \
			ft_isnum \
			ft_isspace \
			ft_memmove \
			ft_memset \
			ft_strchr \
			ft_strdup \
			ft_strjoin \
			ft_strlen \
			ft_strncmp \
			ft_strndup \
			lst
SRCS		:=	$(addprefix $(SRCDIR)/, $(addsuffix .c, $(SRCFILES)))

OBJDIR		:=	obj
OBJS		:=	$(patsubst $(SRCDIR)/%.c, $(OBJDIR)/%.o, $(SRCS))

all:	$(NAME)

$(NAME):	$(OBJS)
	$(AR) $@ $^

$(OBJDIR)/%.o:	$(SRCDIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -I$(INCDIR) -c $< -o $@

clean:
	rm -rf $(OBJDIR)

fclean: clean
	rm -rf $(NAME)

re:	fclean all
.PHONY:	all clean fclean re
