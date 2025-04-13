
#include "minishell.h"

// Utilisé par max, si possible ne pas les modifiers :

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

// Fin des fonctions utilisé par max (1 seul fonction utilisé : free_tab)

void	mini_exit(int ret, t_data *data)
{
	write(1, "exit\n", 5);
	free_tab(data->env);
	exit(ret);
}

void	exit_minishell(char *str, int exit_value, t_data *data)
{
	(void)data;
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(str, 2);
	write(2, "\n", 1);
	free_tab(data->env);
	//free list;
	//free input tab;
	exit(exit_value);
}

void put_error(char *cmd, char *arg, char *error)
{
	write (2, "minishell: ", 12);
	if (cmd)
	{
	ft_putstr_fd(cmd, 2);
	ft_putstr_fd(": ", 2);
	}
	if(arg)
	{
		ft_putstr_fd("'", 2);
		ft_putstr_fd(arg, 2);
		ft_putstr_fd("': ", 2);
	}
	ft_putstr_fd(error, 2);
}
void free_data(t_data *data)
{
	free_tab(data->env);
	//free list;
	//free input tab;
}
