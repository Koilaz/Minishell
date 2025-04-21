#include "../minishell.h"

/****************************************************************
Return 1 si un argument est le nom d;ub built-in
******************************************************************/
int	is_build_in(char *name)
{
	if (!ft_strlcmp("exit", name))
		return (1);
	else if (!ft_strlcmp("export", name))
		return (1);
	else if (!ft_strlcmp("echo", name))
		return (1);
	else if (!ft_strlcmp("unset", name))
		return (1);
	else if (!ft_strlcmp("env", name))
		return (1);
	else if (!ft_strlcmp("pwd", name))
		return (1);
	else if (!ft_strlcmp("cd", name))
		return (1);
	else
		return (0);
}

void exit_init_exec(t_cmd *cmd, t_data *data, char *str)
{
	free_tab(cmd->arg);
	free_tab(cmd->filename_list);
	free(cmd->type_list);
	free(cmd);
	exit_minishell(str, 1, data);
}
void free_cmd(t_cmd *cmd)
{
	free_tab(cmd->arg);
	free_tab(cmd->filename_list);
	free(cmd->type_list);
	free(cmd);
}

char **add_line_cmd(char **tab, char *str, t_data *data, t_cmd *cmd)
{
	char	**ntab;
	int		i;

	if(!str)
		return (tab);
	i = 0;
	ntab = ft_calloc(count_line(tab) + 2, sizeof(char *));
	if (!ntab)
		exit_init_exec(cmd, data,"malloc fail");
	while (tab && tab[i])
	{
		ntab[i] = tab[i];
		i++;
	}
	ntab[i] = ft_strdup(str);
	if (!ntab[i])
	{
		free(ntab);
		exit_init_exec(cmd, data, "malloc fail");
	}
	return (free(tab), ntab);
}