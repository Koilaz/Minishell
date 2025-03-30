
#include "minishell.h"

/****************************************************************
Return 1 si un argument est le nom d;ub built-in
!!  A completer au fur et a mesure qu'on implemente les Build-in
******************************************************************/
int	is_build_in(char *name)
{
	if (!ft_strcmp("exit", name))
		return (1);
	if (!ft_strcmp("export", name))
		return (1);
	else
		return (0);
}

/****************************************************************
execute les fonctions build in correspondant a arg[0] et return leurs return
Si arg[0] ne correspont a aucun buil-in renvoie 0
******************************************************************/
int	built_in_exec(char **arg, t_data *data)
{
	if (!strcmp("exit", arg[0]))
		bi_exit(arg, data);
	if (!strcmp("export", arg[0]))
		return (bi_export(arg, data));
	return (0);
}

/****************************************************************
Initialise les signaux, execute un programmme exterieur dans un process enfant
et remet les signaux par defaut.
******************************************************************/
void	init_exec_extern(char **arg, t_data *data)
{
	pid_t	pid;
	int		status;

	pid = fork();
	if (pid < 0)
		error_exit("Fork failed", -3, data);
	if (pid == 0)
	{
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_DFL);
		exec_extern(arg, data);
	}
	else
	{
		signal(SIGINT, SIG_IGN);
		signal(SIGQUIT, SIG_IGN);
		waitpid(pid, &status, 0);
		if (WIFSIGNALED(status) && WTERMSIG(status) == SIGINT)
			write(1, "\n", 1);
		signal(SIGINT, sigint_handler);
		signal(SIGQUIT, SIG_IGN);
	}
}

/****************************************************************
Execute minishell en initialisant les signaux correctement
en en les remettant par defaut
/!\A FIXER ?
cas ou minishell n'est pas dans le reperoit courant
/!\A FIXER ?
******************************************************************/
int	exec_minishell(char **arg, t_data *data)
{
	pid_t	pid;
	int		status;

	pid = fork();
	if (pid < 0)
		error_exit("Fork failed", -3, data);
	if (pid == 0)
	{
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_DFL);
		execve("./minishell", arg, data->env);
		perror("execve");
		exit(127);
	}
	else
	{
		signal(SIGINT, SIG_IGN);
		signal(SIGQUIT, SIG_IGN);
		waitpid(pid, &status, 0);
		signal(SIGINT, sigint_handler);
		signal(SIGQUIT, SIG_IGN);
	}
	return (0);
}

/****************************************************************
Execute un programme exterieur et verifie qu'il ce trouve dans le chemin
Relatif, Absolue ou dans le PATH de l'environement
Exit(126) si le programme n'existe pas ou la permission denied
******************************************************************/
void	exec_extern(char **arg, t_data *data)
{
	char	*exec_path;
	char	**env_paths;
	int		i;

	if (arg[0][0] == '/' || (arg[0][0] == '.' && arg[0][1] == '/'))
		execve(arg[0], arg, data->env);
	else
	{
		i = 0;
		env_paths = ft_split(get_env("PATH", data->env), ':');
		if (!env_paths)
			error_exit("Path error", -1, NULL);
		while (env_paths[i])
		{
			exec_path = add_chr('/', env_paths[i]);
			exec_path = ft_str_realoc(exec_path, arg[0]);
			execve(exec_path, arg, data->env);
			i++;
		}
	}
	error_exit(arg[0], 126, (void *)arg);
}
