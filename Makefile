SRCS =	parsing.c \
		commands.c \
		prompt.c \
		signal.c \
		utils.c \
		parsingutils.c \
		pipe_management.c \

OBJS = $(SRCS:.c=.o)
CC = cc
CFLAGS = -Wall -Werror -Wextra -g
EXEC = minishell
NAME = $(EXEC)


$(NAME): $(OBJS)
	$(MAKE) -j -C libft/ -s
	$(CC) $(CFLAGS) -o $(EXEC) -lreadline $(OBJS) -Llibft -lft -I./libft

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

all : $(NAME)

clean :
	rm -f $(OBJS)
	$(MAKE) clean -C libft/ -s

fclean : clean
	rm -f $(NAME)
	$(MAKE) fclean -C libft/ -s

re : fclean all

.PHONY : all, clean, fclean, re