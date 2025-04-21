/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leo <leo@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 15:59:45 by lmarck            #+#    #+#             */
/*   Updated: 2025/04/17 09:07:00 by leo              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

// Utilisé par max, si possible ne pas les modifiers :

char *get_env(char *name, char **env)
{
	int i;
	int len;
	char *tmp;
	char *ptr;

	if(!name)
		return (NULL);
	i = 0;
	tmp = ft_strjoin(name, "=");
	len =  ft_strlen(tmp);
	while(env[i])
	{
		ptr = ft_strnstr(env[i], tmp, len);
		if(ptr)
			return (free(tmp), ptr + len);
		i++;
	}
	free(tmp);
	return (NULL);
}


// Fin des fonctions utilisé par max (1 seul fonction utilisé : get_env)


// char *get_env_copy(char *name, char **env)
// {
// 	int		i;
// 	size_t	len;

// 	if (name == NULL || env == NULL)
// 		return (NULL);

// 	len = ft_strlen(name);
// 	i = 0;
// 	while (env[i] != NULL)
// 	{
// 		if (ft_strncmp(env[i], name, len) == 0 && env[i][len] == '=')
// 			return (ft_strdup(env[i] + len + 1));
// 		i++;
// 	}
// 	return (NULL);
// }

int	count_line(char **tab)
{
	int i;
	
	if(!tab)
		return(0);
	i = 0;
	while(tab[i])
		i++;
	return(i);
}

char	*add_chr(unsigned char buf, char *str)
{
	char	*nstr;
	int		i;

	i = 0;
	nstr = ft_calloc(ft_strlen(str) + 2, sizeof(char));
	if (!nstr)
		return (NULL);
	while (str[i])
	{
		nstr[i] = str[i];
		i++;
	}
	nstr[i] = buf;
	nstr[i + 1] = 0;
	free(str);
	return (nstr);
}

char	*ft_str_realoc(char *s1, char const *s2)
{
	char	*ptr;
	size_t	i;
	size_t	j;

	if (!s1 || !s2)
		return (NULL);
	ptr = (char *)malloc(sizeof(char) * (ft_strlen(s1) + ft_strlen(s2) + 1));
	if (ptr == NULL)
		return (NULL);
	i = 0;
	while (s1[i] != '\0')
	{
		ptr[i] = s1[i];
		i++;
	}
	j = 0;
	while (s2[j] != '\0')
	{
		ptr[i + j] = s2[j];
		j++;
	}
	ptr[i + j] = '\0';
	free(s1);
	return (ptr);
}
char **sort_tab(char **tab)
{
	char *tmp;
	int i;
	int j;

	i = 0;
	while(tab[i])
	{
		j = i + 1;
		while(tab[j])
		{
			if (ft_strcmp(tab[i], tab[j]) > 0)
			{
				tmp = tab[i];
				tab[i] = tab[j];
				tab[j] = tmp;
			}
			j++;
		}
		i++;
	}
	return (tab);
}
char **dup_tab(char **tab)
{
	char **ntab;
	int i;

	i = 0;
	ntab = ft_calloc(count_line(tab) + 1, sizeof(char*));
	if (!ntab)
		return (NULL);
	while(tab[i])
	{
		ntab[i] = tab[i];
		i++;
	}
	return (ntab);
}

int ft_is_zero(int n)
{
	if(n)
		return (1);
	return (0);
}
 char **secure_split(char *line, char sep, t_data *data)
{
	char **tab;

	if (!line)
		return (NULL);
	tab = ft_split(line, sep);
	if(!tab)
		exit_minishell("malloc failed", MALLOC_FAIL, data);
	return (tab);
}

/****************************************************************
ft_putstr_fd qui renvoie -1 si write fail et 0 en cas de succes
******************************************************************/
int	ft_putstrs_fd(char *s, int fd)
{
	int	r;

	if (!s)
		return (-1);
	r = write(fd, s, ft_strlen(s));
	if (r >= 0)
		return (0);
	else
		return (-1);
}

/****************************************************************
ft_strcmp qui compare egalement la longueur des string
renvoie -256 si les strings sont simillaires jusqu'a \0
mais de longeur differentes.
******************************************************************/
int	ft_strlcmp(const char *s1, const char *s2)
{
	unsigned char	*str1;
	unsigned char	*str2;
	size_t 			i;

	str1 = (unsigned char *)s1;
	str2 = (unsigned char *)s2;

	i = 0;
	while (s1[i] != '\0' && s2[i] != '\0')
	{
		if (s1[i] != s2[i])
		{
			return (str1[i] - str2[i]);
		}
		i++;
	}
	if(ft_strlen(s1) != ft_strlen(s2))
		return(-256);
	return (0);
}
