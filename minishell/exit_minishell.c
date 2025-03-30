
#include "minishell.h"

void	free_tab(char **tab)
{
	int	i;

	i = 0;
	if (tab == NULL)
		return;
	while (tab[i])
		free(tab[i++]);
	free(tab);
}

void	mini_exit(int ret, t_data *data)
{
	write(1, "exit\n", 5);
	free_tab(data->env);
	exit(ret);
}

void	error_exit(char *str, int exit_value, t_data *data)
{
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(str, 2);
	write(2, " ", 1);
	perror(NULL);
	free_data(data);
	exit(exit_value);
}
void free_data(t_data *data)
{
	if (data->env_paths)
	{
		free_tab(data->env_paths);
		data->env_paths = NULL;
	}
}
void put_error(char *cmd, char *arg, char *error)
{
	write (2, "minishell: ", 12);
	ft_putstr_fd(cmd, 2);
	ft_putstr_fd(": '", 2);
	ft_putstr_fd(arg, 2);
	ft_putstr_fd("': ", 2);
	ft_putstr_fd(error, 2);
}
