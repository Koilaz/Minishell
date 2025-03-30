/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwallis <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/16 19:14:26 by mwallis           #+#    #+#             */
/*   Updated: 2024/10/17 18:55:34 by mwallis          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memcpy(void *dest, const void *src, size_t n)
{
	size_t				i;
	unsigned char		*destination;
	const unsigned char	*source;

	if (!dest && !src)
	{
		return (NULL);
	}
	i = 0;
	destination = dest;
	source = src;
	while (i < n)
	{
		destination[i] = source[i];
		i++;
	}
	return (dest);
}
