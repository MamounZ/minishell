LIBFT_L = -L./libft -lft
READ_LINE_L = -lreadline
LIBFT_PATH = ./libft
CC = cc
CFLAGS = -Wall -Wextra -Werror -g
NAME = minishell
SRCS = main.c envp_copy.c expander.c builtin_cmd.c tokenizer.c tokenizer_utiltes.c export.c unset.c pwd.c cd.c signals.c exu.c exu_2.c mang_pip.c exit.c export_utils.c cd_utils.c expander_len.c expander_utils.c random_utils.c mang_pip_utils.c mang_pip_utils1.c mang_pip_utils2.c mang_pip_utils3.c mang_pip_utils4.c mang_pip_utils5.c mang_pip_utils6.c mang_pip_utils7.c tokenizer_utilites1.c

OBJS = $(SRCS:.c=.o)

all: $(NAME)

$(NAME): $(OBJS)
		make -C $(LIBFT_PATH)
		$(CC) $(CFLAGS) $(OBJS) $(LIBFT_L) $(READ_LINE_L) -o $(NAME)

clean:
		make clean -C $(LIBFT_PATH)
		rm -rf $(OBJS)

fclean: clean
		make fclean -C $(LIBFT_PATH)
		rm -rf $(NAME)

re: fclean all

.PHONY: all clean fclean re
