/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvan-slu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/04 16:00:53 by lvan-slu          #+#    #+#             */
/*   Updated: 2023/11/19 15:22:57 by lvan-slu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strchr(const char *s, int c)
{
	char			*str;
	unsigned char	b;
	int				i;
	int				j;

	str = (char *) s;
	b = c;
	j = ft_strlen(str);
	i = 0;
	while (i <= j)
	{
		if (str[i] == b)
			return (&str[i]);
		i++;
	}
	return (0);
}
