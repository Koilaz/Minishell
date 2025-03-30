/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwallis <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/21 17:17:11 by mwallis           #+#    #+#             */
/*   Updated: 2024/10/21 17:31:50 by mwallis          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_atoi(const char *nptr)
{
	int		i;
	int		sign;
	int		result;
	char	*nbletter;

	i = 0;
	sign = 1;
	result = 0;
	nbletter = (char *)nptr;
	while (nbletter[i] == 32 || (nbletter[i] >= 9 && nbletter[i] <= 13))
		i++;
	if (nbletter[i] == '+' || nbletter[i] == '-')
	{
		if (nbletter[i] == '-')
			sign = sign * -1;
		i++;
	}
	while (nbletter[i] >= '0' && nbletter[i] <= '9')
	{
		result = (result * 10) + (nbletter[i] - 48);
		i++;
	}
	return (sign * result);
}
