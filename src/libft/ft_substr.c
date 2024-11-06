/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvan-slu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/10 16:44:54 by lvan-slu          #+#    #+#             */
/*   Updated: 2023/11/13 15:52:19 by lvan-slu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	char	*dest;
	size_t	i;
	size_t	j;

	if (!s)
		return (NULL);
	if (ft_strlen(s) < start)
	{
		dest = (char *)malloc(sizeof(char) * 1);
		if (dest == NULL)
			return (0);
		*dest = '\0';
		return (dest);
	}
	if (len + start > ft_strlen(s))
		len = ft_strlen(s) - start;
	dest = (char *)malloc(sizeof(char) * len + 1);
	if (dest == NULL)
		return (NULL);
	i = start;
	j = 0;
	while (s[i] != '\0' && j < len)
		dest[j++] = s[i++];
	dest[j] = '\0';
	return (dest);
}
