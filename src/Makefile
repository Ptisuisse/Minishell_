SRCS =	main.c \
		builtins_cd_pwd.c \
		builtins_exit.c \
		builtins_echo.c \
		builtins_env.c \
		builtins_export_utils.c \
		builtins_export.c \
		dollar.c \
		parsingutils.c \
		parsing.c \
		parsing_redirections.c \
		commands.c \
		redirection_manager.c \
		redirection_error.c \
		env_var.c \
		error.c \
		execution_manager.c \
		execution_utils.c \
		execution_with_pipe.c \
		execution_without_pipe.c \
		signal.c \
		utils.c \
		heredoc.c \
		append_file.c \
		redirection.c \
		parsing_commands.c \
		multiple_redirections.c \
		check_files.c \
		builtins_exit_utils.c \
		heredoc_utils.c \
		ft_free.c \


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