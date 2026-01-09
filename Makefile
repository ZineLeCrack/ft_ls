NAME = ft_ls

CC = cc
CFLAGS = -Wall -Wextra -Werror -g3 -Iincludes

SRCDIR = srcs
LIBFTDIR = libft
OBJDIR = objs

SRCS = get_dir_info.c handle_directories.c print_content.c list_option.c utils.c main.c
SRCS := $(addprefix $(SRCDIR)/, $(SRCS))

LIBFTSRCS =	ft_atoi.c ft_bzero.c ft_calloc.c ft_isalnum.c ft_isalpha.c \
			ft_isascii.c ft_isdigit.c ft_isprint.c ft_itoa.c \
			ft_lstadd_back_bonus.c ft_lstadd_front_bonus.c \
			ft_lstclear_bonus.c ft_lstdelone_bonus.c \
			ft_lstiter_bonus.c ft_lstlast_bonus.c \
			ft_lstmap_bonus.c ft_lstnew_bonus.c \
			ft_lstsize_bonus.c ft_memchr.c ft_memcmp.c \
			ft_memcpy.c ft_memmove.c ft_memset.c \
			ft_putchar_fd.c ft_putendl_fd.c ft_putnbr_fd.c \
			ft_putstr_fd.c ft_split.c ft_strchr.c \
			ft_strdup.c ft_striteri.c ft_strjoin.c \
			ft_strlcat.c ft_strlcpy.c ft_strlen.c \
			ft_strmapi.c ft_strncmp.c ft_strnstr.c \
			ft_strrchr.c ft_strtrim.c ft_substr.c \
			ft_tolower.c ft_toupper.c ft_strcmp.c \
			ft_printf/ft_printf.c ft_printf/ft_putchar.c ft_printf/ft_puthexa.c \
			ft_printf/ft_puthex.c ft_printf/ft_putnbr.c ft_printf/ft_putnbr_unsigned.c \
			ft_printf/ft_putstr.c

LIBFTSRCS := $(addprefix $(LIBFTDIR)/, $(LIBFTSRCS))

OBJS =	$(SRCS:%.c=$(OBJDIR)/%.o) \
		$(LIBFTSRCS:%.c=$(OBJDIR)/%.o)

all: $(NAME)

$(OBJDIR):
	mkdir -p $(OBJDIR)/$(SRCDIR) $(OBJDIR)/$(LIBFTDIR)

$(OBJDIR)/%.o: %.c
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME)
	@echo "\033[32m✔ Compilation completed\033[0m"

clean:
	$(RM) -r $(OBJDIR)

fclean: clean
	$(RM) $(NAME)

re: fclean all
