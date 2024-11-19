/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strncmp.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lisambet <lisambet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/19 15:25:44 by lvan-slu          #+#    #+#             */
/*   Updated: 2024/11/19 17:08:27 by lisambet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_strncmp(const char *s1, const char *s2, size_t n)
{
	size_t			a;
	unsigned char	*str1;
	unsigned char	*str2;

	a = 0;
	if (!s1)
		return (1);
	str1 = (unsigned char *)s1;
	str2 = (unsigned char *)s2;
	while (str1[a] != '\0' && a < n)
	{
		if (str1[a] != str2[a])
			return (str1[a] - str2[a]);
		a++;
	}
	if ((a < n && a == 0) || (str2[a] != '\0' && a < n))
		return (-str2[a]);
	return (0);
}
