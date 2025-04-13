/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwallis <mwallis@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 22:56:39 by mwallis           #+#    #+#             */
/*   Updated: 2025/04/10 22:00:08 by mwallis          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static void	clean_hostname(char *str)
{
	char	*newline;
	char	*dot;

	newline = ft_strchr(str, '\n');
	if (newline != NULL)
		*newline = '\0';
	dot = ft_strchr(str, '.');
	if (dot != NULL)
		*dot = '\0';
}

static char	*get_host(void)
{
	int		fd;
	char	*str;
	int		len;

	fd = open("/etc/hostname", O_RDONLY);
	if (fd < 0)
		return (NULL);
	str = malloc(sizeof(char) * 256);
	if (str == NULL)
		return (close(fd), NULL);
	len = read(fd, str, 255);
	close(fd);
	if (len <= 0)
		return (free(str), NULL);
	str[len] = '\0';
	clean_hostname(str);
	return (str);
}

static char	*shorten_pwd(char *pwd, char *home, t_data *data, char *host)
{
	char	*tmp;

	if (pwd != NULL && home != NULL && ft_strncmp(pwd, home, ft_strlen(home)) == 0)
	{
		tmp = ft_strjoin("~", pwd + ft_strlen(home));
		free(pwd);

		// free(tmp);
		// tmp = NULL;

		if (tmp == NULL)
		{
			free(host);
			write(2, ERROR_PROMPT, 65);
			rl_clear_history();
			free_tab(data->env);
			exit (1);
		}
	
		return (tmp);
	}
	return (pwd);
}

static char	*build_user_host(char *user, char *host)
{
	char	*tmp;
	char	*user_host;

	tmp = ft_strjoin(user, "@");


	// free(tmp);
	// tmp = NULL;

	if (tmp == NULL)
	{
		return (free(host), NULL);
	}
	user_host = ft_strjoin(tmp, host);
	free(tmp);
	free(host);


	// free(user_host);
	// user_host = NULL;


	if (user_host == NULL)
	{
		return (NULL);
	}


	tmp = ft_strjoin(user_host, ":");
	free(user_host);


	// 	free(tmp);
	// tmp = NULL;

	if (tmp == NULL)
	{
		return (NULL);
	}
	user_host = ft_strjoin(GRN_PROMPT, tmp);
	free(tmp);


	// free(user_host);
	// user_host = NULL;

	if (user_host == NULL)
	{
		return (NULL);
	}
	return (user_host);
}

char	*get_prompt(t_data *data)
{
	char	*user;
	char	*host;
	char	*pwd;
	char	*colored;
	char	*tmp;

	user = get_env("USER", data->env);
	host = get_host();
	pwd = getcwd(NULL, 0);
	
	if (pwd == NULL)
		pwd = ft_strdup("/");


	pwd = shorten_pwd(pwd, get_env("HOME", data->env), data, host);
	// if (user == NULL || pwd == NULL || host == NULL || 1 == 1)
	if (user == NULL || pwd == NULL || host == NULL)
		return (free(pwd), free(host), ft_strdup(GRN_PROMPT "minishell > " RESET_PROMPT));


	colored = build_user_host(user, host);
	if (colored == NULL)
		return (free(pwd), NULL);


	tmp = ft_strjoin(BLU_PROMPT, pwd);
	free(pwd);

	if (tmp == NULL)
		return (free(colored), NULL);

	pwd = ft_strjoin(colored, tmp);
	free(colored);
	free(tmp);

	if (pwd == NULL)
		return (NULL);

	colored = ft_strjoin(pwd, RESET_PROMPT "$ ");

	free(pwd);


	// free(colored);
	// colored = NULL;

	if (colored == NULL)
		return (NULL);

	// Vraiment besoin de ceci ? au cas ou :
	//colored = add_chr('\0', colored);

	return (colored);
}
