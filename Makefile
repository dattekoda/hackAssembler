NAME	:=	HackAssembler
CC	:=	cc
CFLAGS	:=	-Wall -Wextra -Werror

INCDIR	:=	include

SRCDIR		:=	src
SRCFILES	:=	main \
			ft_get_next_line \
			ft_strlen \
			ft_strncmp \
			ft_memmove \
			ft_strjoin \
			ft_strchr \
			ft_strndup \
			ft_strdup \
			ft_isspace \
			lst
SRCS		:=	$(addprefix $(SRCDIR)/, $(addsuffix .c, $(SRCFILES)))

OBJDIR	:=	obj
OBJS	:=	$(patsubst $(SRCDIR)/%.c, $(OBJDIR)/%.o, $(SRCS))

all:	$(NAME)
$(NAME):	$(OBJS)
	$(CC) $(CFLAGS) $^ -o $@

$(OBJDIR)/%.o:	$(SRCDIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -I$(INCDIR) -c $< -o $@

clean:
	rm -rf $(OBJDIR)
fclean:	clean
	rm -rf $(NAME)
re:	fclean all

.PHONY: all clean fclean re
