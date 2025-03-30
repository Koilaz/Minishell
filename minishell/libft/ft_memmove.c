/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwallis <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/17 16:38:29 by mwallis           #+#    #+#             */
/*   Updated: 2024/10/18 15:23:40 by mwallis          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memmove(void *dest, const void *src, size_t n)
{
	unsigned char		*destination;
	const unsigned char	*source;
	size_t				i;

	if (!dest && !src)
		return (NULL);
	destination = dest;
	source = src;
	i = 0;
	if (source < destination && destination < source + n)
	{
		i = n;
		while (i-- > 0)
			destination[i] = source[i];
	}
	else
	{
		while (i < n)
		{
			destination[i] = source[i];
			i++;
		}
	}
	return (dest);
}
