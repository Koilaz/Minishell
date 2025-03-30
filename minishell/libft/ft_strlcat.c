/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwallis <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/18 16:29:09 by mwallis           #+#    #+#             */
/*   Updated: 2024/10/24 17:45:40 by mwallis          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcat(char *dst, const char *src, size_t size)
{
	size_t	lensource;
	size_t	lendest;
	size_t	i;

	if ((!dst && size <= 0) || (!src && size <= 0))
		return (0);
	lensource = ft_strlen(src);
	lendest = ft_strlen(dst);
	if (size <= lendest)
	{
		return (size + lensource);
	}
	i = 0;
	while (src[i] != '\0' && (lendest + i) < (size - 1))
	{
		dst[lendest + i] = src[i];
		i++;
	}
	dst[lendest + i] = '\0';
	return (lendest + lensource);
}
