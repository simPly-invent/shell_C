NAME = minishell
CC = cc
CFLAGS = -Wall -Wextra -Werror
RL_CFLAGS = -lreadline

# Directories
SRC_DIR = src
INC_DIR = includes
LIBFT_DIR = libft

# Source files
SRCS = $(SRC_DIR)/main.c $(SRC_DIR)/shell.c $(SRC_DIR)/signals.c        $(SRC_DIR)/executor/executor.c $(SRC_DIR)/executor/execve.c        $(SRC_DIR)/executor/pipe.c $(SRC_DIR)/executor/redir.c        $(SRC_DIR)/executor/builtins.c $(SRC_DIR)/executor/builtins_cd.c        $(SRC_DIR)/executor/builtins_echo.c $(SRC_DIR)/executor/builtins_env.c        $(SRC_DIR)/executor/builtins_exit.c $(SRC_DIR)/executor/builtins_export.c        $(SRC_DIR)/executor/builtins_pwd.c $(SRC_DIR)/executor/builtins_unset.c        $(SRC_DIR)/parser/parser.c $(SRC_DIR)/parser/expand.c        $(SRC_DIR)/parser/expand_utils.c $(SRC_DIR)/parser/redir.c        $(SRC_DIR)/parser/tokenizer.c $(SRC_DIR)/parser/tokenizer_utils.c        $(SRC_DIR)/utils/env.c $(SRC_DIR)/utils/free.c $(SRC_DIR)/utils/utils.c

OBJS = $(SRCS:.c=.o)

# Libft
LIBFT = $(LIBFT_DIR)/libft.a

all: $(NAME)

$(NAME): $(OBJS) $(LIBFT)
	$(CC) $(CFLAGS) $(OBJS) $(LIBFT) $(RL_CFLAGS) -o $(NAME)

%.o: %.c
	$(CC) $(CFLAGS) -I$(INC_DIR) -I$(LIBFT_DIR)/includes -c $< -o $@

$(LIBFT):
	$(MAKE) -C $(LIBFT_DIR)

clean:
	rm -f $(OBJS)
	$(MAKE) -C $(LIBFT_DIR) clean

fclean: clean
	rm -f $(NAME)
	$(MAKE) -C $(LIBFT_DIR) fclean

re: fclean all

.PHONY: all clean fclean re
