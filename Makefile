LIBFT_L = -L./libft -lft
READ_LINE_L = -lreadline
LIBFT_PATH = ./libft
CC = cc
CFLAGS = -Wall -Wextra -Werror
NAME = minishell
SRCS = main.c envp_copy.c #error_exit.c expander.c

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
