
#include "../../minishell.h"

void sigint_handler(int signum)
{
    (void)signum;
    write(1, "\n", 1);
    rl_on_new_line();       // readline sait qu'une ligne vide commence
    rl_replace_line("", 0); // efface la ligne en cours
    rl_redisplay();         // réaffiche le prompt
}

int	is_str_only_numeric(const char *str)
{
    int i;

    i = 0;
	if (str == NULL || str[i] == '\0')
		return (0);
	while (str[i] != '\0')
	{
		if (ft_isdigit(str[i]) == 0)
			return (0);
		i++;
	}
	return (1);
}

char *increase_shlvl(char *old_shell_lvl)
{
	int		shlvl_int;
	char	*shlvl_str;
	char	*new_shlvl;

	if (is_str_only_numeric(old_shell_lvl + 6) == 0)
	{
		write(2, "minishell: warning: shell level (SHLVL) invalid, resetting to 1\n", 64);
		return (ft_strdup("SHLVL=1"));
	}
	shlvl_int = ft_atoi(old_shell_lvl + 6);

	if (shlvl_int >= 999 || shlvl_int < 0)
	{
		write(2, "minishell: warning: shell level (SHLVL) too high or invalid, resetting to 1\n", 76);
		return (ft_strdup("SHLVL=1"));
	}

	shlvl_int++;
	shlvl_str = ft_itoa(shlvl_int);
	if (shlvl_str == NULL)
		return (NULL);
	new_shlvl = ft_strjoin("SHLVL=", shlvl_str);

    // free(new_shlvl);
    // new_shlvl = NULL;
    if (new_shlvl == NULL)
    {
        free(shlvl_str);
        return (NULL);
    }
	free(shlvl_str);
	return (new_shlvl);
}


char **init_minimal_env(void)
{
    char **env_minimal;
    char *pwd;
    char *tmp;

    env_minimal = malloc(sizeof(char *) * 5);
    if (env_minimal == NULL)
        return (NULL);

    pwd = getcwd(NULL, 0);
    if (pwd == NULL)
        pwd = ft_strdup("/");
    if (pwd == NULL)
    {
        free(env_minimal);
        return (NULL);
    }
    tmp = ft_strjoin("PWD=", pwd);
    free(pwd);
    if (tmp == NULL)
        return (free(env_minimal), NULL);
    env_minimal[0] = tmp;
    env_minimal[1] = ft_strdup("SHLVL=1");
    if (env_minimal[1] == NULL)
        return (free(env_minimal[0]), free(env_minimal), NULL);
    env_minimal[2] = ft_strdup("_=/usr/bin/env");
    if (env_minimal[2] == NULL)
    {
        free(env_minimal[0]);
        free(env_minimal[1]);
        free(env_minimal);
        return (NULL);
    }
    env_minimal[3] = ft_strdup("PATH=/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin");
    if (env_minimal[3] == NULL)
    {
        free(env_minimal[0]);
        free(env_minimal[1]);
        free(env_minimal[2]);
        free(env_minimal);
        return (NULL);
    }

    env_minimal[4] = NULL;
    return (env_minimal);
}

char **add_to_env(char **env, char *new_var)
{
	int		i;
    int     len;
	char	**new_env;

    // ici
    // free(new_var);
    // new_var = NULL;


	if (new_var == NULL)
        return (free_tab(env), NULL);

	len = 0;
	while (env != NULL && env[len] != NULL)
		len++;
	new_env = malloc(sizeof(char *) * (len + 2));

    // ici
    // free(new_env);
    // new_env = NULL;

	if (new_env == NULL)
        return (free_tab(env), free(new_var), NULL);

    i = 0;
    while (i < len)
    {
        new_env[i] = env[i];
        i++;
    }

	new_env[i] = new_var;
	new_env[i + 1] = NULL;
	free(env);
	return (new_env);
}

char    **complete_env_if_needed(char **env)
{
    char *pwd;
    char *pwd_full_line;

    //if (get_env("AZER", env) == NULL)
    if (get_env("PWD", env) == NULL)
    {
        pwd = getcwd(NULL, 0);
        if (pwd == NULL)
            pwd = ft_strdup("/");
        if (pwd == NULL)
            return (free_tab(env), NULL);
        pwd_full_line = ft_strjoin("PWD=", pwd);
        free(pwd);
        // if (pwd_full_line == NULL)
        //     return (free_tab(env), NULL);
        env = add_to_env(env, pwd_full_line);
        if (env == NULL)
            return (NULL);
    }
    // if (get_env("MERVEILLEUX", env) == NULL)
    if (get_env("SHLVL", env) == NULL)
    {
        env = add_to_env(env, ft_strdup("SHLVL=1"));
        if (env == NULL)
            return (NULL);
    }
    if (get_env("_", env) == NULL)
    {
        env = add_to_env(env, ft_strdup("_=/usr/bin/env"));
        if (env == NULL)
            return (NULL);
    }
    if (get_env("PATH", env) == NULL)
    {
        env = add_to_env(env, ft_strdup("PATH=/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin"));
        if (env == NULL)
            return (NULL);
    }
    return (env);
}

char **copy_env(char **envp)
{
    char    **env;
    int     i;

    i = 0;
    if (envp == NULL || envp[i] == NULL)
    {
        env = init_minimal_env();
        return (env);
    }

    while (envp[i] != NULL)
        i++;
    env = malloc(sizeof(char *) * (i + 1));
    if (env == NULL)
        return (NULL);
    i = 0;
    while (envp[i] != NULL)
    {
        if (ft_strncmp(envp[i], "SHLVL=", 6) == 0)
        {
            env[i] = increase_shlvl(envp[i]);
            if (env[i] == NULL)
            {
                while (i > 0)
                    free(env[--i]);
                free(env);
                return (NULL);
            }
        }
        else
        {
            env[i] = ft_strdup(envp[i]);


            // if (i == 6)
            // {
            //     free(env[i]);
            //     env[i] = NULL;
            // }

            if (env[i] == NULL)
            {
                while (i > 0)
                    free(env[--i]);
                free(env);
                return (NULL);
            }
        }
        i++;
    }
    env[i] = NULL;
    env = complete_env_if_needed(env);
    return (env);
}

int minishell_init(char **envp, t_data *data)
{
	signal(SIGINT, sigint_handler); // gérer CTRL+C
	signal(SIGQUIT, SIG_IGN);       // ignorer CTRL+
    data->last_exit_status = 0;
	data->env = copy_env(envp);
	if (data->env == NULL)
		return (write (2, ERROR_ENV, 72), -1);
	return (0);
}
