CC = cc
CFLAGS = -Wall -Wextra -Werror -Iincludes
NAME = libft.a
ARFLAGS = rcs
SRCS = ft_toupper.c ft_tolower.c ft_isalpha.c ft_memset.c ft_bzero.c ft_atoi.c ft_isdigit.c \
	ft_strchr.c ft_isalnum.c ft_isascii.c ft_isprint.c ft_memchr.c ft_memcmp.c \
	ft_strncmp.c ft_strlen.c ft_strrchr.c ft_memcpy.c ft_memmove.c ft_strncmp.c ft_strlcpy.c \
	ft_strlcat.c ft_calloc.c ft_strdup.c ft_strnstr.c ft_substr.c ft_strjoin.c ft_strtrim.c \
	ft_strmapi.c ft_striteri.c ft_putchar_fd.c ft_putstr_fd.c ft_putendl_fd.c ft_putnbr_fd.c \
	ft_itoa.c ft_split.c ft_strcmp.c ft_printf.c ft_putadrs.c ft_putchar.c ft_puthex.c ft_putnbr.c ft_putstr.c ft_putunbr.c \
	get_next_line.c get_next_line_utils.c 
SRCSB = ft_lstnew_bonus.c ft_lstadd_front_bonus.c ft_lstsize_bonus.c ft_lstlast_bonus.c \
	ft_lstadd_back_bonus.c ft_lstdelone_bonus.c ft_lstclear_bonus.c ft_lstiter_bonus.c ft_lstmap_bonus.c \
	get_next_line_bonus.c get_next_line_utils_bonus.c

OBJS = $(SRCS:.c=.o)
OBJS_B = $(SRCSB:.c=.o)
#creating the statico librere
all: $(NAME)
#now we compile the source files (build them and turning them into objects)
#$< means the %.c (requirements) and the $@ means the target which is %.o
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@
$(NAME): $(OBJS)
	ar $(ARFLAGS) $@ $?
bonus: $(OBJS_B) $(NAME)
	ar ${ARFLAGS} ${NAME} ${OBJS_B}  
#here we remove the object files (.o) that were created
clean:
	rm -rf ${OBJS} ${OBJS_B}
#we just remove the library files here
fclean: clean
	rm -rf ${NAME}
#here we rebuild everything, by deleting and redoing all of the steps again
re: fclean all
#
.PHONY: all bonus clean fclean re
