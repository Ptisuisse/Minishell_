/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvan-slu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/10 16:40:49 by lvan-slu          #+#    #+#             */
/*   Updated: 2023/11/18 14:47:12 by lvan-slu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strtrim(char const *s1, char const *set)
{
	size_t	start;
	size_t	end;
	char	*dest;

	if (!set || !s1)
		return (0);
	start = 0;
	end = ft_strlen(s1);
	while (s1[start] != '\0' && (ft_strchr(set, s1[start]) != 0))
		start++;
	while ((ft_strchr(set, s1[end - 1]) != 0) && end > 0)
		end--;
	if (end < start)
		return (ft_calloc(1, 1));
	dest = ft_substr(s1, start, (end - start));
	return (dest);
}
