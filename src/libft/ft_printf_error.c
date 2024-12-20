/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvan-slu <lvan-slu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/25 13:24:52 by lvan-slu          #+#    #+#             */
/*   Updated: 2024/03/21 18:07:43 by lvan-slu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	print_char_error(int c)
{
	return (write(2, &c, 1));
}

int	print_str_error(char *str)
{
	int	i;

	i = 0;
	if (str == NULL)
	{
		write(2, "(null)", 6);
		return (i + 6);
	}
	while (*str != '\0')
	{
		print_char_error((int)*str);
		i++;
		str++;
	}
	return (i);
}

int	print_type_error(char type, va_list ap)
{
	int	i;

	i = 0;
	if (type == 'c')
		i += print_char_error(va_arg(ap, int));
	else if (type == 's')
		i += print_str_error(va_arg(ap, char *));
	else if (type == 'p')
		i += print_add_error(va_arg(ap, unsigned long long), 16);
	else if (type == 'd')
		i += print_digit_error((long)va_arg(ap, int), 10, type);
	else if (type == 'i')
		i += print_digit_error((long)va_arg(ap, int), 10, type);
	else if (type == 'u')
		i += print_digit_error((long)va_arg(ap, unsigned int), 10, type);
	else if (type == 'x')
		i += print_digit_error((long)va_arg(ap, unsigned int), 16, type);
	else if (type == 'X')
		i += print_digit_error((long)va_arg(ap, unsigned int), 16, type);
	else if (type == '%')
		i += write(2, "%", 1);
	return (i);
}

int	ft_printf_error(const char *format, ...)
{
	va_list	ap;
	int		i;

	va_start(ap, format);
	i = 0;
	while (*format != '\0')
	{
		if (*format == '%')
			i += print_type_error(*(++format), ap);
		else
			i += write(2, format, 1);
		format++;
	}
	va_end(ap);
	return (i);
}
