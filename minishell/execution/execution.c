
#include "../minishell.h"

/****************************************************************
execute la command de cmd_arg[0] et renvoie le retour de la commande execute
******************************************************************/
int	exec_command(t_cmd *cmd, t_data *data)
{
	if (is_build_in(cmd->arg[0]))
	{	
		set_redirection_parent(cmd, data);
		return (data->last_exit_status);
	}
	if (ft_strlcmp(cmd->arg[0], "./minishell") == 0)
		return (exec_minishell(cmd, data));
	else
		init_exec_extern(cmd, data);
	return (1);
}

/****************************************************************
execute les fonctions build in correspondant a arg[0] et return leurs return
Si arg[0] ne correspont a aucun buil-in renvoie 0
******************************************************************/
int	built_in_exec(t_cmd *cmd, t_data *data)
{
	if (!ft_strlcmp("exit", cmd->arg[0]))
		bi_exit(cmd->arg, data);
	if (!ft_strlcmp("export", cmd->arg[0]))
		return (bi_export(cmd->arg, data));
	if (!ft_strlcmp("echo", cmd->arg[0]))
		return (bi_echo(cmd->arg, 1, data));
	if (!ft_strlcmp("unset", cmd->arg[0]))
		return (bi_unset(cmd->arg, data));
	if (!ft_strlcmp("env", cmd->arg[0]))
		return (bi_env(cmd->arg, data));
	if (!ft_strlcmp("pwd", cmd->arg[0]))
		return (bi_pwd(cmd->arg, data));
	if (!ft_strlcmp("cd", cmd->arg[0]))
		return (bi_cd(cmd->arg, data));
	return (0);
}

/****************************************************************
Initialise les signaux, execute un programmme exterieur dans un process enfant
et remet les signaux par defaut.
******************************************************************/
void	init_exec_extern(t_cmd *cmd, t_data *data)
{
	pid_t	pid;
	int		status;

	pid = fork();
	if (pid < 0)
		exit_init_exec(cmd, data, "fork fail");
	if (pid == 0)
	{
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_DFL);
		set_redirection(cmd, data, -1);
		exec_extern(cmd, data);
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
		free_cmd(cmd);
	}
}

/****************************************************************
Execute minishell en initialisant les signaux correctement
en en les remettant par defaut
/!\A FIXER ?
cas ou minishell n'est pas dans le reperoit courant
/!\A FIXER ?
******************************************************************/
int	exec_minishell(t_cmd *cmd, t_data *data)
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
		set_redirection(cmd, data, -1);
		execve("./minishell", cmd->arg, data->env);
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


void	exec_extern(t_cmd *cmd, t_data *data)
{
	char	*exec_path;
	char	**env_paths;
	int		i;

	if (strchr(cmd->arg[0], '/'))
		execve(cmd->arg[0], cmd->arg, data->env);
	else
	{
		i = 0;
		env_paths = secure_split(get_env("PATH", data->env), ':', data);;
		while (env_paths[i])
		{
			exec_path = add_chr('/', env_paths[i]);
			exec_path = ft_str_realoc(exec_path, cmd->arg[0]);
			execve(exec_path, cmd->arg, data->env);
			i++;
		}
	}
	put_error(NULL, cmd->arg[0], "command not found\n");
	free_data(data);
	free_cmd(cmd);
	exit(126);
}
