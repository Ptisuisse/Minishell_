/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_digit.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvan-slu <lvan-slu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/28 17:43:08 by lvan-slu          #+#    #+#             */
/*   Updated: 2024/03/21 18:05:24 by lvan-slu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	print_digit_error(long n, int base, char type)
{
	int		i;
	char	*symbol;

	i = 0;
	if (type == 'X')
		symbol = "0123456789ABCDEF";
	else
		symbol = "0123456789abcdef";
	if (n < 0)
	{
		write(1, "-", 1);
		return (print_digit_error(-n, base, type) + 1);
	}
	else if (n < base)
		return (print_char(symbol[n]));
	else
	{
		i = print_digit_error((n / base), base, type);
		return (i + print_digit_error((n % base), base, type));
	}
}
