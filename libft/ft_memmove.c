/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvan-slu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/04 15:16:09 by lvan-slu          #+#    #+#             */
/*   Updated: 2023/11/13 20:48:14 by lvan-slu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memmove(void *dest, const void *src, size_t n)
{
	unsigned char	*dst;
	unsigned char	*str;

	dst = (unsigned char *)dest;
	str = (unsigned char *)src;
	if (src > dest)
		ft_memcpy(dest, src, n);
	else if (dest > src)
	{
		while (n--)
		{
			dst[n] = str[n];
		}
	}
	return (dst);
}
