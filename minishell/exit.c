
#include"minishell.h"

void	bi_exit(char **arg, t_data *data) //
{
	int	v;
	int	i;
	int	ret;

	i = 0;
	if (count_line(arg) > 2)
	{
		write (2, "minishell: exit: too many arguments\n", 37);
		mini_exit(1, data);
	}
	while (arg[++i])
	{
		ret = ft_atoi_with_validation(arg[i], &v);
		if (!v)
		{
			write (2, "minishell: exit: ", 18);
			ft_putstr_fd(arg[1], 2);
			write (2, ": numeric argument required\n", 29);
			mini_exit(2, data);
		}
	}
	if (arg[1] && v)
		mini_exit(ret % 256, data);
	else
		mini_exit(0, data);
}

int	ft_atoi_with_validation(const char *nptr, int *is_valid)
{
	int		i;
	int		sign;
	long	result;
	i = 0;
	sign = 1;
	result = 0;
	*is_valid = 1;
	while (nptr[i] == 32 || (nptr[i] >= 9 && nptr[i] <= 13))
		i++;
	if (nptr[i] == '+' || nptr[i] == '-')
	{
		if (nptr[i++] == '-')
			sign = sign * -1;
	}
	while (nptr[i] >= '0' && nptr[i] <= '9')
	{
		result = (result * 10) + (nptr[i++] - 48);
		if ((sign == 1 && result > INT_MAX)
			|| (sign == -1 && (-1 * result) < INT_MIN))
			return (*is_valid = 0, 0);
	}
	if (nptr[i] != '\0')
		*is_valid = 0;
	return (sign * result);
}
