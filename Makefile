SRCS =	main.c \
		builtins_cd_pwd.c \
		builtins_clear_exit.c \
		builtins_echo.c \
		builtins_env.c \
		builtins_export_utils.c \
		builtins_export.c \
		command_management.c \
		dollar.c \
		env_var.c \
		error.c \
		execution.c \
		parsing.c \
		parsingutils.c \
		redirect_management.c \
		redirections.c \
		signal.c \
		utils.c \
		heredoc.c \
		append_file.c \
		commands.c \
		redirection_error.c \
		parsing_redirections.c \
		parsing_commands.c \

OBJS = $(SRCS:.c=.o)
CC = cc
CFLAGS = -Wall -Werror -Wextra -g
EXEC = minishell
NAME = $(EXEC)


$(NAME): $(OBJS)
	$(MAKE) -j -C libft/ -s
	$(CC) $(CFLAGS) -o $(EXEC) $(OBJS) -Llibft -lft -I./libft -lreadline

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