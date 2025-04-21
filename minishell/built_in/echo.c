/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leo <leo@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 14:49:03 by lmarck            #+#    #+#             */
/*   Updated: 2025/04/16 18:48:01 by leo              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	bi_echo(char **arg, int i, t_data *data)
{
	int	n;

	if (!arg[1])
		return (ft_putstrs_fd("\n", 1));
	n = ft_strcmp(arg[1], "-n");
	if (!n)
		i++;
	while (arg[i])
	{
		if (ft_putstrs_fd(arg[i], 1) < 0)
			exit_minishell("write fail", WRITE_FAIL, data);
		if (arg[i + 1])
		{
			if (ft_putstrs_fd(" ", 1) < 0)
				exit_minishell("write fail", WRITE_FAIL, data);
		}
		i++;
	}
	if (n)
	{
		if (write(1, "\n", 1) < 0)
			return (-1);
	}
	return (0);
}
