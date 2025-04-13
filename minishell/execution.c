
#include "minishell.h"

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

/****************************************************************
execute les fonctions build in correspondant a arg[0] et return leurs return
Si arg[0] ne correspont a aucun buil-in renvoie 0
******************************************************************/
int	built_in_exec(char **arg, t_data *data)
{
	if (!ft_strlcmp("exit", arg[0]))
		bi_exit(arg, data);
	if (!ft_strlcmp("export", arg[0]))
		return (bi_export(arg, data));
	if (!ft_strlcmp("echo", arg[0]))
		return (bi_echo(arg, 1, data));
	if (!ft_strlcmp("unset", arg[0]))
		return (bi_unset(arg, data));
	if (!ft_strlcmp("env", arg[0]))
		return (bi_env(arg, data));
	if (!ft_strlcmp("pwd", arg[0]))
		return (bi_pwd(arg, data));
	if (!ft_strlcmp("cd", arg[0]))
		return (bi_cd(arg, data));
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
		exit_minishell("fork fail", FORK_FAIL, data);
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
		data->last_exit_status = WEXITSTATUS(status);
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
		exit_minishell("fork fail", FORK_FAIL, data);
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
		data->last_exit_status = WEXITSTATUS(status);
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


// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

// Faire un check avec access et lstat car ici c'est faux

// Non si execve ne marche pas il met l;erreur de perror, essaie ca fonctionne.
// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!


void	exec_extern(char **arg, t_data *data)
{
	char	*exec_path;
	char	**env_paths;
	int		i;

	if (strchr(arg[0], '/'))
		execve(arg[0], arg, data->env);
	else
	{
		i = 0;
		env_paths = secure_split(get_env("PATH", data->env), ':', data);;
		while (env_paths[i])
		{
			exec_path = add_chr('/', env_paths[i]);
			exec_path = ft_str_realoc(exec_path, arg[0]);
			execve(exec_path, arg, data->env);
			i++;
		}
	}
	put_error(NULL, arg[0], "command not found\n");
	free_data(data);
	exit(126);
}
