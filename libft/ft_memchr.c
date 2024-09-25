/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvan-slu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/07 16:13:44 by lvan-slu          #+#    #+#             */
/*   Updated: 2023/11/19 15:27:22 by lvan-slu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memchr(const void *s, int c, size_t n)
{
	size_t			i;
	unsigned char	*str;
	unsigned char	cpy;

	i = 0;
	str = (unsigned char *) s;
	cpy = (unsigned char) c;
	while (i < n)
	{
		if (str[i] == cpy)
			return (&str[i]);
		i++;
	}
	return (0);
}
