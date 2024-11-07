SRCS =  ft_atoi \
        ft_bzero \
        ft_isalnum \
        ft_isalpha \
        ft_isascii \
        ft_isdigit \
        ft_isprint \
        ft_memset \
	ft_memcpy \
	ft_memmove \
	ft_memchr \
	ft_memcmp \
        ft_strlen \
        ft_tolower \
        ft_toupper \
	ft_strlcpy \
	ft_strlcat \
	ft_strchr \
	ft_strrchr \
	ft_strncmp \
	ft_strdup \
	ft_substr \
	ft_calloc \
	ft_putchar_fd \
	ft_putstr_fd \
	ft_putendl_fd \
	ft_putnbr_fd \
	ft_strnstr  \
	ft_strjoin \
	ft_striteri \
	ft_strmapi \
	ft_itoa \
	ft_strtrim \
	ft_split \
	get_next_line \
	ft_printf \
	print_add \
	print_digit \
	free_strjoin \
	ft_printf_error \
	print_add_error \
	print_digit_error \

SRCB =	ft_lstnew \
	ft_lstadd_front \
	ft_lstsize \
	ft_lstlast \
	ft_lstadd_back \
	ft_lstdelone \
	ft_lstiter \
	ft_lstclear \
	ft_lstmap \

CC = cc

CFLAGS = -Wall -Werror -Wextra

EXEC = libft.a

CFILES = $(SRCS:%=%.c)

CBONUS = $(SRCB:%=%.c)

OBJ = $(SRCS:=.o)

OBJB = $(SRCB:=.o)

NAME = $(EXEC)

$(NAME):
	$(CC) $(CFLAGS) -c $(CFILES) -I./
	ar -rc $(EXEC) $(OBJ)

all : $(NAME)

bonus :
	$(CC) $(CFLAGS) -c $(CBONUS) -I./
	ar -rc $(EXEC) $(OBJB)

clean :
	rm -f $(OBJ) $(OBJB)

fclean : clean
	rm -f $(NAME)

re : fclean all

.PHONY : all, clean, fclean, re
