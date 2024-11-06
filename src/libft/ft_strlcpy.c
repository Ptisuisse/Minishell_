/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcpy.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvan-slu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/05 11:01:20 by lvan-slu          #+#    #+#             */
/*   Updated: 2023/11/13 20:51:00 by lvan-slu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcpy(char *dst, const char *src, size_t size)
{
	size_t	j;
	char	*str;

	str = (char *)src;
	j = 0;
	if (size == 0)
		return (ft_strlen(str));
	while (j < (size - 1) && str[j] != '\0')
	{
		dst[j] = str[j];
		j++;
	}
	dst[j] = '\0';
	while (str[j] != '\0')
		j++;
	return (j);
}
