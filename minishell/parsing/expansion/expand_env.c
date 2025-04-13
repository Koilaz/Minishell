#include "../../minishell.h"


int is_valid_brace_expansion_syntax(char *str)
{
    int i;

    i = 0;

	if (str == NULL || str[i] == '\0')
		return (0);
	if (ft_isalpha(str[i]) == 0 && str[i] != '_')
		return (0);
    if (ft_strcmp(str, "?") == 0 || ft_strcmp(str, "$") == 0)
		return (1);
    while (str[i] != '\0')
    {
        if (ft_isalnum(str[i]) == 0 && str[i] != '_')
			return (0);
        i++;
    }
	return (1);
}


char  *expand_env_variables(char *full_str, t_data *data, int *status_code)
{
    int     i;
    int     start;
    char    *str;
    char    *new_full_str;
    char    *tmp;

    i = 0;
    start = 0;
    str = NULL;
 
    // full_str = NULL;
    if (full_str == NULL)
    {
        *status_code = 99;
        return (NULL);
    }
    new_full_str = ft_strdup("");

    // free(new_full_str);
    // new_full_str = NULL;

    if (new_full_str == NULL)
    {
        *status_code = 2;
        return (NULL);
    }
    while (full_str[i] != '\0')
    {
        if (full_str[i] == '$' && full_str[i + 1] == '{')
        {
            i = i + 2;
            start = i;
            while (full_str[i] != '\0' && full_str[i] != '}')
                i++;
            if (full_str[i] != '}')
            {
                // Bonus : Laisser utilisateur finir } avec heredoc mais pas necessaire
                write(2, ERROR_MISSING_BRACE, 41);
                free(new_full_str);
                *status_code = 1;
                data->last_exit_status = 2;
                return (NULL);
            }
            str = ft_strndup(full_str + start, i - start);

            // free(str);
            // str = NULL;
            if (str == NULL)
            {
                // printf("gtz");
                free(new_full_str);
                *status_code = 2;
                return (NULL);
            }
            i++;

            if (ft_strcmp(str, "?") == 0)
            {
                tmp = ft_itoa(data->last_exit_status);

                // free(tmp);
                // tmp = NULL;

                if (tmp == NULL)
                {
                    free(str);
                    free(new_full_str);
                    *status_code = 2;
                    return (NULL);
                }
            }
            else if (ft_strcmp(str, "$") == 0)
            {
                //interdit dans le sujet, pas reussi a remplacer getpid partout. que sur linux
                // tmp = ft_itoa(getpid());
                write (2, ERROR_PID_EXPAND, 64);
                free(str);
                free(new_full_str);
                *status_code = 1;
                data->last_exit_status = 127;
                return (NULL);
            }

            else if (is_valid_brace_expansion_syntax(str) == 0)
            {
                write(2, "minishell: ${", 13);
                write(2, str, ft_strlen(str));
                write(2, "}: bad substitution\n", 21);
                free(str);
                free(new_full_str);
                *status_code = 1;
                data->last_exit_status = 1;
                return (NULL);
            }
            else
            {
                tmp = get_env(str, data->env);
                if (tmp == NULL)
                {
                    tmp = ft_strdup("");
                    if (tmp == NULL)
                    {
                        free(str);
                        free(new_full_str);
                        *status_code = 2;
                        return (NULL);
                    }
                }
                else
                {
                    tmp = ft_strdup(tmp);
                    // free(tmp);
                    // tmp = NULL;
                    if (tmp == NULL)
                    {
                        free(str);
                        free(new_full_str);
                        *status_code = 2;
                        return (NULL);
                    }
                }
            }
            free(str);
            str = strjoin_safe(new_full_str, tmp);
            free(new_full_str);
            free(tmp);


            // free(str);
            // str = NULL;

            if (str == NULL)
            {
                *status_code = 2;
                return (NULL);
            }
            new_full_str = str;
        }
        else if (full_str[i] == '$')
        {
            i++;

            if (full_str[i] == '?') 
            {
                tmp = ft_itoa(data->last_exit_status);

                // free(tmp);
                // tmp = NULL;

                if (tmp == NULL)
                {
                    free(new_full_str);
                    *status_code = 2;
                    return (NULL);
                }
                str = strjoin_safe(new_full_str, tmp);
                free(tmp);
                free(new_full_str);

                // free(str);
                // str = NULL;
                if (str == NULL)
                {
                    *status_code = 2;
                    return (NULL);
                }
                new_full_str = str;
                i++;
            } 
            else if (full_str[i] == '$') 
            {   
                //interdit dans le sujet, pas reussi a remplacer getpid partout. que sur linux
                // tmp = ft_itoa(getpid());
                // str = strjoin_safe(new_full_str, tmp);
                // free(tmp);
                // free(new_full_str);
                // new_full_str = str;
                // i++;


                write (2, ERROR_PID_EXPAND, 64);
                free(new_full_str);
                *status_code = 1;
                data->last_exit_status = 127;
                return (NULL);
            }
            else if (ft_isdigit(full_str[i]) == 1)
            {
                char number[2];
                
                number[0] = full_str[i];
                number[1] = '\0';
                tmp = get_env(number, data->env);
                i++;


                if (tmp == NULL)
                {
                    tmp = ft_strdup("");

                    // free(tmp);
                    // tmp = NULL;

                    if (tmp == NULL)
                    {
                        free(new_full_str);
                        *status_code = 2;
                        return (NULL);
                    }
                    str = strjoin_safe(new_full_str, tmp);

                    // free(str);
                    // str = NULL;

                    free(tmp);
                    if (str == NULL)
                    {
                        free(new_full_str);
                        *status_code = 2;
                        return (NULL);
                    }
                }
                else 
                {
                    str = strjoin_safe(new_full_str, tmp);

                    // free(str);
                    // str = NULL;

                    if (str == NULL)
                    {
                        free(new_full_str);
                        *status_code = 2;
                        return (NULL);
                    }
                }
                free(new_full_str);
                new_full_str = str;
            }
            else if (full_str[i] == '\0' || (ft_isalnum(full_str[i]) == 0 && full_str[i] != '_'))
            {
                tmp = ft_strdup("$");

                // free(tmp);
                // tmp = NULL;

                if (tmp == NULL)
                {
                    free(new_full_str);
                    *status_code = 2;
                    return (NULL);
                }
                str = strjoin_safe(new_full_str, tmp);

                // free(str);
                // str = NULL;

                free(tmp);
                free(new_full_str);
                if (str == NULL)
                {
                    *status_code = 2;
                    return (NULL);
                }
                new_full_str = str;
            }
            else 
            {
                start = i;
                while (full_str[i] != '\0' && (ft_isalnum(full_str[i]) == 1 || full_str[i] == '_'))
                    i++;
                str = ft_strndup(full_str + start, i - start);

                // free(str);
                // str = NULL;

                if (str == NULL)
                {
                    free(new_full_str);
                    *status_code = 2;
                    return (NULL);
                }
                tmp = get_env(str, data->env);
                free(str);
                if (tmp == NULL)
                {
                    tmp = ft_strdup("");


                    // free(tmp);
                    // tmp = NULL;

                    if (tmp == NULL)
                    {
                        free(new_full_str);
                        *status_code = 2;
                        return (NULL);
                    }
                    str = strjoin_safe(new_full_str, tmp);

                    // free(str);
                    // str = NULL;

                    free(tmp);
                    if (str == NULL)
                    {
                        free(new_full_str);
                        *status_code = 2;
                        return (NULL);
                    }
                    
                }
                else
                {
                    str = strjoin_safe(new_full_str, tmp);

                    // free(str);
                    // str = NULL;

                    if (str == NULL)
                    {
                        free(new_full_str);
                        *status_code = 2;
                        return (NULL);
                    }
                }
                free(new_full_str);
                new_full_str = str;
            }
        }
        else 
        {
            start = i;
            while (full_str[i] != '\0' && full_str[i] != '$')
                i++;
            str = ft_strndup(full_str + start, i - start);

            // free(str);
            // str = NULL;

            if (str == NULL)
            {
                free(new_full_str);
                *status_code = 2;
                return (NULL);
            }
            tmp = strjoin_safe(new_full_str, str);
            free(new_full_str);
            free(str);


            // free(tmp);
            // tmp = NULL;


            if (tmp == NULL)
            {
                *status_code = 2;
                return (NULL);
            }
            new_full_str = tmp;
        }
    }
    return (new_full_str);
}



int    apply_env_expansion(t_segment	*segment_list, t_data *data)
{
    t_segment	*segment_head;
    t_word_fragments *current;
    char *expand_str;
    int status_code;
        
    status_code = 0;
    segment_head = segment_list;
    while (segment_list != NULL)
    {
        if (segment_list->type == WORD)
        {
            current = segment_list->content;
            while (current != NULL)
            {
                if (current->quote_type == QUOTE_NONE ||
                    current->quote_type == QUOTE_DOUBLE)
                {
                    expand_str = expand_env_variables(current->str, data, &status_code);
                    if (expand_str == NULL && status_code == 1)
                    {
                        free_segment_list(&segment_head);
                        return (1);
                    }
                    else if (expand_str == NULL && status_code == 2)
                    {
                        write(2, ERROR_EXPAND_MALLOC, 76);
                        free_segment_list(&segment_head);
                        rl_clear_history();
                        free_tab(data->env);
                        exit (1);
                    }
                    else if (expand_str == NULL && status_code == 99)
                    {
                        // Ceci est juste pour etre ultra robuste. Ca ne peut pas arriver.
                        // Cas impossible sauf bug interne ou corruption mémoire.
                        // On préfère exit immédiatement, même si il est impossible de free la liste doublement chainé.
                        // on ne peut plus faire confiance à la structure une fois qu'un seul élément est cassé.
                        // C'est théoriquement impossible car tout a été vérifié en amont mais c'est une bonne chose pour etre tres robuste.
                        write (2, ERROR_INTERNAL_NULL_EXPAND, 76);
                        free_segment_list(&segment_head);
                        rl_clear_history();
                        free_tab(data->env);
                        exit (99);
                    }

                    char *new_str = ft_strdup(expand_str);

                    // free(new_str);
                    // new_str = NULL;
                    
                    if (new_str == NULL)
                    {
                        free(expand_str);
                        free_segment_list(&segment_head);
                        rl_clear_history();
                        free_tab(data->env);
                        exit (1);
                    }
                    free(current->str);
                    current->str = new_str;
                    free(expand_str);
                }
                current = current->next;
            }
        }
        segment_list = segment_list->next;
    }
    return (0);
}








// Ancien code : 

/*
int is_valid_brace_expansion_syntax(char *str)
{
	int i;

    i = 0;
	if (str == NULL || (ft_isalpha(str[0]) == 0 && str[0] != '_'))
		return (0);
	while (str[i] != '\0')
	{
		if (ft_isalnum(str[i]) == 0 && str[i] != '_')
			return (0);
		i++;
	}
	return (1);
}


char  *expand_env_variables(char *full_str, char **env)
{
    int     i;
    int     start;
    char    *str;
    char    *new_full_str;
    char    *tmp;

    i = 0;
    start = 0;
    str = NULL;
    if (full_str == NULL)
        return (ft_strdup(""));
    new_full_str = ft_strdup("");
    while (full_str[i] != '\0')
    {
        //Rajouter ici cas special $? et $$ Rajouter int last_exit_status; dans t_data
        // if (full_str[i] == '$' && full_str[i + 1] == '$')
        // if (full_str[i] == '$' && full_str[i + 1] == '?')

        if (full_str[i] == '$' && full_str[i + 1] == '{')
        {
            i = i + 2;
            start = i;
            while (full_str[i] != '\0' && full_str[i] != '}')
                i++;
            if (full_str[i] != '}')
            {
                write(2, "minishell: bad substitution\n", 29);
                free(new_full_str);
                return (NULL);
            }
            str = ft_strndup(full_str + start, i - start);
            i++;
            if (is_valid_brace_expansion_syntax(str) == 0)
            {
                write(2, "minishell: ${", 13);
                write(2, str, ft_strlen(str));
                write(2, "}: bad substitution\n", 21);
                free(str);
                free(new_full_str);
                return (NULL);
            }
            tmp = get_env(str, env);
            free(str);
            if (tmp == NULL)
            {
                tmp = ft_strdup("");
                str = strjoin_safe(new_full_str, tmp);
                free(tmp);
            }
            else
            {
                str = strjoin_safe(new_full_str, tmp);
            }
            free(new_full_str);
            new_full_str = str;
        }
        else if (full_str[i] == '$')
        {
            i++;

            if (full_str[i] == '\0' || (ft_isalnum(full_str[i]) == 0 && full_str[i] != '_'))
            {
                tmp = ft_strdup("$");
                str = strjoin_safe(new_full_str, tmp);
                free(tmp);
                free(new_full_str);
                new_full_str = str;
            }
            else 
            {
                start = i;
                while (full_str[i] != '\0' && (ft_isalnum(full_str[i]) == 1 || full_str[i] == '_'))
                    i++;
                str = ft_strndup(full_str + start, i - start);
                tmp = get_env(str, env);
                free(str);
                if (tmp == NULL)
                {
                    tmp = ft_strdup("");
                    str = strjoin_safe(new_full_str, tmp);
                    free(tmp);
                }
                else
                {
                    str = strjoin_safe(new_full_str, tmp);
                }
                free(new_full_str);
                new_full_str = str;
            }
        }
        else 
        {
            start = i;
            while (full_str[i] != '\0' && full_str[i] != '$')
                i++;
            str = ft_strndup(full_str + start, i - start);
            tmp = strjoin_safe(new_full_str, str);
            free(new_full_str);
            free(str);
            new_full_str = tmp;
        }
    }
    return (new_full_str);
}



void    apply_env_expansion(t_segment	*segment_list, t_data data)
{
    char *expand_str;
    t_word_fragments *current;

    while (segment_list != NULL)
    {
        if (segment_list->type == WORD)
        {
            current = segment_list->content;
            while (current != NULL)
            {
                if (current->quote_type == QUOTE_NONE ||
                    current->quote_type == QUOTE_DOUBLE)
                {
                    expand_str = expand_env_variables(current->str, data.env);
                    if (expand_str == NULL)
                    {
                        current = current->next;
                        continue;
                    }
                    free(current->str);
                    current->str = ft_strdup(expand_str);
                    free(expand_str);
                }
                current = current->next;
            }
        }
        segment_list = segment_list->next;
    }
}
*/

// Old
// int is_valid_brace_expansion_syntax(char *str)
// {
// 	int i;

//     i = 0;
// 	if (str == NULL || (ft_isalpha(str[0]) == 0 && str[0] != '_'))
// 		return (0);
// 	while (str[i] != '\0')
// 	{
// 		if (ft_isalnum(str[i]) == 0 && str[i] != '_')
// 			return (0);
// 		i++;
// 	}
// 	return (1);
// }