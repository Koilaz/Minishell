/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmarck <lmarck@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 15:59:45 by lmarck            #+#    #+#             */
/*   Updated: 2025/03/29 17:23:42 by lmarck           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	count_line(char **tab)
{
	int i;

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
char *get_env(char *val, char **env)
{
	int i;
	int len;
	char *tmp;
	char *ret;

	if(!val)
		return (NULL);
	i = 0;
	tmp = ft_strjoin(val, "=");
	len =  ft_strlen(tmp);
	while(env[i])
	{
		ret = ft_strnstr(env[i], tmp, len);
		if(ret)
			return (free(tmp), ret + len);
		i++;
	}
	free(tmp);
	return (NULL);
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
char **secure_split(char *line, char sep)
{
	char **tab;

	if (!line)
		return (NULL);
	tab = ft_split(line, sep);
	if(!tab)
		exit(MALLOC_FAIL);
	else
		return (tab);
}
