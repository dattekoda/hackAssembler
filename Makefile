NAME	:=	HackAssembler
CC	:=	cc
CFLAGS	:=	-Wall -Wextra -Werror

INCFLAGS	:=	-Iinclude -Ilib/include

SRCDIR		:=	src
SRCFILES	:=	main \
			isValidArg \
			readFileToLst \
			assembleHack \
			setHackLine \
			error \
			setInfo \
			hash \
			predefinedSymbol \
			setLabelSymbol
SRCS		:=	$(addprefix $(SRCDIR)/, $(addsuffix .c, $(SRCFILES)))

OBJDIR	:=	obj
OBJS	:=	$(patsubst $(SRCDIR)/%.c, $(OBJDIR)/%.o, $(SRCS))

LIBDIR	:=	lib
LIBA	:=	$(LIBDIR)/lib.a

all:	$(NAME)
$(NAME):	$(OBJS) $(LIBA)
	$(CC) $(CFLAGS) $^ -o $@

$(OBJDIR)/%.o:	$(SRCDIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(INCFLAGS) -c $< -o $@

$(LIBA):
	make -C $(dir $@)

clean:
	make fclean -C $(LIBDIR)
	rm -rf $(OBJDIR)
fclean:	clean
	rm -rf $(NAME)
re:	fclean all

.PHONY: all clean fclean re
