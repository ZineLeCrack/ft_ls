NAME = ft_ls

CC = cc
CFLAGS = -Wall -Wextra -Werror -g3 -Iincludes

SRCDIR = srcs
OBJDIR = objs
LIBFT_DIR = libft
LIBFT = $(LIBFT_DIR)/libft.a

SRCS = help_message.c get_dir_info.c handle_directories.c print_content.c list_option.c utils.c main.c
SRCS := $(addprefix $(SRCDIR)/, $(SRCS))

OBJS = $(SRCS:%.c=$(OBJDIR)/%.o)

all: $(LIBFT) $(NAME)

$(LIBFT):
	$(MAKE) -C $(LIBFT_DIR)

$(OBJDIR):
	mkdir -p $(OBJDIR)

$(OBJDIR)/%.o: %.c
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) $(LIBFT) -o $(NAME)
	@echo "\033[32m✔ Compilation completed\033[0m"

clean:
	$(MAKE) -C $(LIBFT_DIR) clean
	$(RM) -r $(OBJDIR)

fclean: clean
	$(MAKE) -C $(LIBFT_DIR) fclean
	$(RM) $(NAME)

re: fclean all
