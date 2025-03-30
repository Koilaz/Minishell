
#include "minishell.h"


int init_minishell(char **envp, t_data *data)
{
	signal(SIGINT, sigint_handler); // gérer CTRL+C
	signal(SIGQUIT, SIG_IGN);       // ignorer CTRL+
	data->env = copy_env(envp);
	if (data->env == NULL)
		return (write (2, ERROR_ENV, 45), 1);
	return (0);
}


char *increase_shlvl(char *shlvl_line)
{
    int shlvl;
    char *lvl_str;
    char *new_shlvl;

    shlvl = ft_atoi(shlvl_line + 6);
    shlvl++;
    lvl_str = ft_itoa(shlvl);
    if (!lvl_str)
        return (NULL);
    new_shlvl = ft_strjoin("SHLVL=", lvl_str);
    free(lvl_str);
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
            if (!env[i])
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
    return (env);
}

void sigint_handler(int signum)
{
    (void)signum;
    write(1, "\n", 1);
    rl_on_new_line();       // readline sait qu'une ligne vide commence
    rl_replace_line("", 0); // efface la ligne en cours
    rl_redisplay();         // réaffiche le prompt
}
