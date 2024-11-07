/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_add.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvan-slu <lvan-slu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/28 17:44:33 by lvan-slu          #+#    #+#             */
/*   Updated: 2024/03/21 18:05:44 by lvan-slu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	print_pointeur_error(unsigned long long n, unsigned long long base)
{
	int		i;
	char	*symbol;

	i = 0;
	symbol = "0123456789abcdef";
	if (n < base)
		return (print_char_error(symbol[n]));
	else
	{
		i = print_pointeur_error((n / base), base);
		return (i + print_pointeur_error((n % base), base));
	}
}

int	print_add_error(unsigned long long n, unsigned long long base)
{
	int	i;

	i = 0;
	if (n == 0)
		return (i += write(1, "(nil)", 5));
	else
	{
		i += write(1, "0x", 2);
		return (i += print_pointeur_error(n, base));
	}
}
