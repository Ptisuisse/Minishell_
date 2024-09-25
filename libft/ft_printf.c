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

int	print_char(int c)
{
	return (write(1, &c, 1));
}

int	print_str(char *str)
{
	int	i;

	i = 0;
	if (str == NULL)
	{
		write (1, "(null)", 6);
		return (i + 6);
	}
	while (*str != '\0')
	{
		print_char((int)*str);
		i++;
		str++;
	}
	return (i);
}

int	print_type(char type, va_list ap)
{
	int	i;

	i = 0;
	if (type == 'c')
		i += print_char(va_arg(ap, int));
	else if (type == 's')
		i += print_str(va_arg(ap, char *));
	else if (type == 'p')
		i += print_add(va_arg(ap, unsigned long long), 16);
	else if (type == 'd')
		i += print_digit((long)va_arg(ap, int), 10, type);
	else if (type == 'i')
		i += print_digit((long)va_arg(ap, int), 10, type);
	else if (type == 'u')
		i += print_digit((long)va_arg(ap, unsigned int), 10, type);
	else if (type == 'x')
		i += print_digit((long)va_arg(ap, unsigned int), 16, type);
	else if (type == 'X')
		i += print_digit((long)va_arg(ap, unsigned int), 16, type);
	else if (type == '%')
		i += write(1, "%", 1);
	return (i);
}

int	ft_printf(const char *format, ...)
{
	va_list	ap;
	int		i;

	va_start(ap, format);
	i = 0;
	while (*format != '\0')
	{
		if (*format == '%')
			i += print_type(*(++format), ap);
		else
			i += write(1, format, 1);
		format++;
	}
	va_end(ap);
	return (i);
}
