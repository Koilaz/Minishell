#include "../../minishell.h"


t_segment	*create_segment(void)
{
    t_segment   *segment;

    segment = (t_segment *)malloc(sizeof(t_segment));
    if (segment == NULL)
        return (NULL);

    segment->index = 0;
    segment->content = NULL;
    segment->type = SEGMENT_UNSET;
    segment->are_there_quotes_inside = 0;
    segment->next = NULL;
    segment->previous = NULL;

	return (segment);
}

void	segment_add_back(t_segment **segment_list, t_segment *new_segment)
{
	t_segment	*current;

	if (segment_list == NULL || new_segment == NULL)
		return ;
	if (*segment_list == NULL)
	{
		*segment_list = new_segment;
		return ;
	}
	current = *segment_list;
	while (current->next != NULL)
	{
		current = current->next;
	}
    new_segment->index = (current->index + 1);
	current->next = new_segment;
	new_segment->previous = current;
}

// void    reassign_segment_list_index(t_segment *segment_list)
// {
//     int i;

//     i = 0;

//     while (segment_list != NULL)
//     {
//         segment_list->index = i;
//         i++;
//         segment_list = segment_list->next;
//     }
// }

// void	free_segment_list(t_segment **segment)
// {
// 	t_segment	*tmp_segment;
//     t_word_fragments *tmp_fragment;

// 	while (*segment != NULL)
// 	{
// 		tmp_segment = (*segment)->next;
//         if ((*segment)->content != NULL)
//         {
//             while ((*segment)->content != NULL)
//             {
//                 tmp_fragment = (*segment)->content->next;
//                 if ((*segment)->content->str != NULL)
//                     free((*segment)->content->str);
//                 free((*segment)->content);
//                 (*segment)->content = tmp_fragment;
//             }
//         }
// 		free(*segment);
// 		*segment = tmp_segment;
// 	}
// }


void    free_word_fragments(t_word_fragments *fragment)
{
    t_word_fragments *tmp;

    while (fragment != NULL)
    {
        tmp = fragment->next;
        if (fragment->str != NULL)
            free(fragment->str);
        free(fragment);
        fragment = tmp;
    }
}

void    free_segment_list(t_segment **segment)
{
    t_segment *tmp;

    while (*segment != NULL)
    {
        tmp = (*segment)->next;
        if ((*segment)->content != NULL)
            free_word_fragments((*segment)->content);
        free(*segment);
        *segment = tmp;
    }
}

t_word_fragments   *create_fragment(char *str, t_quote_type quote_type)
{
    t_word_fragments   *fragment;

    fragment = (t_word_fragments *)malloc(sizeof(t_word_fragments));
    if (fragment == NULL)
        return (NULL);

    fragment->str = str;
    fragment->quote_type = quote_type;
    fragment->next = NULL;
    fragment->previous = NULL;

    return (fragment);
}

void    fragment_add_back(t_word_fragments **fragments_list, t_word_fragments *new_fragment)
{
    t_word_fragments   *current;

    if (fragments_list == NULL || new_fragment == NULL)
        return ;
    if (*fragments_list == NULL)
    {
        *fragments_list = new_fragment;
        return ;
    }
    current = *fragments_list;
    while (current->next != NULL)
    {
        current = current->next;
    }
    current->next = new_fragment;
    new_fragment->previous = current;
}


int     skip_white_space(char *str)
{
    int    i;

    i = 0;
    while (str[i] == ' ' || (str[i] >= 9 && str[i] <= 13))
    {
        i++;
    }
    return (i);
}

void print_segment_list(t_segment *segment_list)
{
    t_word_fragments *fragments;
    char quote[4];

    printf("\033[1;34m========== Liste des segments ==========\033[0m\n");
    while (segment_list != NULL)
    {
        printf("\033[1;32mSegment #%d\033[0m  \033[1;32mType:\033[0m %d\n",
               segment_list->index, segment_list->type);
        printf("\033[1;36mFragments:\033[0m [");
        fragments = segment_list->content;
        while (fragments != NULL)
        {
            if (fragments->quote_type == QUOTE_SINGLE)
                printf("\033[31m'\033[0m%s\033[31m'\033[0m", fragments->str);
            else if (fragments->quote_type == QUOTE_DOUBLE)
                printf("\033[31m\"\033[0m%s\033[31m\"\033[0m", fragments->str);
            else
                printf("%s", fragments->str);
            if (fragments->next != NULL)
                printf(" \033[1;33m|\033[0m ");
            fragments = fragments->next;
        }
        printf("]\n");
        if (segment_list->are_there_quotes_inside)
            ft_strlcpy(quote, "Oui", sizeof(quote));
        else
            ft_strlcpy(quote, "Non", sizeof(quote));
        printf("\033[1;33mContient des quotes:\033[0m %s\n", quote);
        printf("\033[1;34m----------------------------------------\033[0m\n");
        segment_list = segment_list->next;
    }
}


static int is_separator(char c)
{
    if (c == ' ' || c == '\t' || c == '\n' || c == '\v' || c == '\f' || c == '\r' || c == '|' || c == '<' || c == '>')
        return (1);
    return (0);
}

void	lexer_error_exit(char *origin, t_segment **segment_list, t_data *data)
{
	write(2, ERROR_LEXER, 72);
	free(origin);
	free_segment_list(segment_list);
	rl_clear_history();
	free_tab(data->env);
	exit(1);
}

t_word_fragments *split_word_into_fragments(const char *str, int *chars_read, int *are_quotes_closed)
{
    t_word_fragments *fragment_list;
    t_word_fragments *fragment;
    char *fragment_str;
    int i;
    int start;
    int len;
    t_quote_type quote_type;
    
    
    i = 0;
    fragment_list = NULL;

    while (str[i] != '\0' && is_separator(str[i]) == 0)
    {
        start = i;
        len = 0;
        quote_type = QUOTE_NONE;
            
        if (str[i] == '\'' || str[i] == '\"')
        {
            if (str[i] == '\'')
                quote_type = QUOTE_SINGLE;
            else
                quote_type = QUOTE_DOUBLE;

            i++;
            start = i;

            while (str[i] != '\0' && ((quote_type == QUOTE_SINGLE && str[i] != '\'') ||
                              (quote_type == QUOTE_DOUBLE && str[i] != '\"')))
                i++;

            if (str[i] == '\0')
            {
                *are_quotes_closed = -1;
                free_word_fragments(fragment_list);
                fragment_list = NULL;
                break;
            }

            len = i - start;
            i++;
        }

        else
        {
            while (str[i] != '\0' && is_separator(str[i]) == 0 && str[i] != '\'' && str[i] != '\"')
                i++;
            len = i - start;
        }
        
        fragment_str = ft_strndup(str + start, len);

        // free(fragment_str);
        // fragment_str = NULL;

        if (fragment_str == NULL)
        {
            free_word_fragments(fragment_list);
            return (NULL);
        }
        
        fragment = create_fragment(fragment_str, quote_type);

        // free(fragment);
        // fragment = NULL;


        if (fragment == NULL)
        {
            free(fragment_str);
	        free_word_fragments(fragment_list);
	        return (NULL);
        }
        fragment_add_back(&fragment_list, fragment);
    }
    
    *chars_read = i;
    return (fragment_list);
}

int add_word_segment(char *str, t_segment **segment_list, char *origin, t_data *data)
{
    t_segment *segment;
    t_word_fragments *fragment;
    int i;
    int are_quotes_closed;

    segment = create_segment();

    // free_segment_list(&segment);
    // segment = NULL;


    if (segment == NULL)
        lexer_error_exit(origin, segment_list, data);

    i = 0;
    are_quotes_closed = 0;
    segment->content = split_word_into_fragments(str, &i, &are_quotes_closed);

    if (segment->content == NULL && are_quotes_closed == -1)
    {
        segment->type = ERROR_QUOTE;
        segment_add_back(segment_list, segment);
        return (i);
    }
    else if (segment->content == NULL)
    {
        free(segment);
        lexer_error_exit(origin, segment_list, data);
    }
    fragment = segment->content;
    while (fragment != NULL)
    {
        if (fragment->quote_type == QUOTE_SINGLE || fragment->quote_type == QUOTE_DOUBLE)
            segment->are_there_quotes_inside = 1;
        fragment = fragment->next;
    }
    segment->type = WORD;
    segment_add_back(segment_list, segment);
    return (i);
}


static int add_double_operator_segment(char *str, t_segment **segment_list, char *origin, t_data *data)
{
    t_segment *new_segment;

    if (str[0] == '<' && str[1] != '\0' && str[1] == '<')
    {
        new_segment = create_segment();

        // free_segment_list(&new_segment);
        // new_segment = NULL;

        if (new_segment == NULL)
            lexer_error_exit(origin, segment_list, data);
        new_segment->type = HEREDOC;
        segment_add_back(segment_list, new_segment);
        return (2);
    }
    else if (str[0] == '>' && str[1] != '\0' && str[1] == '>')
    {
        new_segment = create_segment();
        if (new_segment == NULL)
            lexer_error_exit(origin, segment_list, data);
        new_segment->type = APPEND;
        segment_add_back(segment_list, new_segment);
        return (2);
    }
    return (0);
}

static int add_single_operator_segment(char *str, t_segment **segment_list, char *origin, t_data *data)
{
    t_segment *new_segment;

    if (str[0] == '<')
    {
        new_segment = create_segment();
        if (new_segment == NULL)
            lexer_error_exit(origin, segment_list, data);
        new_segment->type = INPUT_REDIRECTION;
        segment_add_back(segment_list, new_segment);
        return (1);
    }
    else if (str[0] == '>')
    {
        new_segment = create_segment();
        if (new_segment == NULL)
            lexer_error_exit(origin, segment_list, data);
        new_segment->type = OUTPUT_REDIRECTION;
        segment_add_back(segment_list, new_segment);
        return (1);
    }
    else if (str[0] == '|')
    {
        new_segment = create_segment();
        if (new_segment == NULL)
            lexer_error_exit(origin, segment_list, data);
        new_segment->type = PIPE;
        segment_add_back(segment_list, new_segment);
        return (1);
    }
    return (0);
}

int add_operator_segment(char *str, t_segment **segment_list, char *origin, t_data *data)
{
    int chars_read;

    chars_read = 0;
    chars_read = add_double_operator_segment(str, segment_list, origin, data);
    if (chars_read > 0)
        return (chars_read);
    chars_read = add_single_operator_segment(str, segment_list, origin, data);
    return (chars_read);
}


t_segment   *lexer(char *input_line, t_data *data)
{
    t_segment   *segment_list;
    t_segment   *segment_end;

    char    *origin;
    
    int     i;

    origin = input_line;

    segment_list = NULL;
    segment_end = NULL;

    i = 0;
    while (input_line[i] != '\0')
    {
        i += skip_white_space(&input_line[i]);
        if (input_line[i] == '\0')
            break;
        //i += skip_white_space(&input_line[i]);
        if (input_line[i] == '|' || input_line[i] == '<' || input_line[i] == '>')
            i += add_operator_segment(&input_line[i], &segment_list, origin, data);
        else
            i += add_word_segment(&input_line[i], &segment_list, origin, data);
    }

    segment_end = create_segment();

    // free_segment_list(&segment_end);
    // segment_end = NULL;

    if (segment_end == NULL)
        lexer_error_exit(origin, &segment_list, data);
    segment_end->type = END_OF_FILE;
    segment_add_back(&segment_list, segment_end);
    free(origin);
    return (segment_list);
}


//Ancienne version au cas ou :

// typedef struct s_quote_flags
// {
//     int in_single_quote;
//     int in_double_quote;
//     int are_quotes_closed;
// }	t_quote_flags;

// void	print_segment_list(t_segment *segment_list)
// {
// 	while (segment_list != NULL)
// 	{
// 		printf("[%d] type: %d | str: %s\n", segment_list->index, segment_list->type,
// 			segment_list->str ? segment_list->str : "(null)");
            
//         printf("simple : %d| double : %d\n\n", segment_list->is_single_quote, segment_list->is_double_quote);

//         segment_list = segment_list->next;
// 	}
// }

// static int count_quoted_characters(const char *str, int *i, char quote, int *len)
// {
//     while (str[*i] != '\0' && str[*i] != quote)
//     {
//         (*len)++;
//         (*i)++;
//     }
//     if (str[*i] != quote)
//         return (-1);
//     (*i)++;
//     return (0);
// }

// static int get_word_length(const char *str, int *i, t_quote_flags *quote_flags)
// {
//     int len;
//     char quote;

//     len = 0;
//     quote_flags->in_single_quote = 0;
//     quote_flags->in_double_quote = 0;
//     quote_flags->are_quotes_closed = 0;
//     while (str[*i] != '\0' && is_separator(str[*i]) == 0)
//     {
//         if (str[*i] == '\'' || str[*i] == '\"')
//         {
//             quote_flags->are_there_quotes_inside = 1;
//             quote = str[(*i)++];
//             if (count_quoted_characters(str, i, quote, &len) == -1)
//                 return (-1);
//         }
//         else
//         {
//             len++;
//             (*i)++;
//         }
//     }
//     quote_flags->are_quotes_closed = 1;
//     return (len);
// }

// static int write_word_segment(const char *str, char *word)
// {
//     int i;
//     int j;
//     char quote;

//     i = 0;
//     j = 0;
//     while (str[i] != '\0' && is_separator(str[i]) == 0)
//     {
//         if (str[i] == '\'' || str[i] == '\"')
//         {
//             quote = str[i++];
//             while (str[i] != '\0' && str[i] != quote)
//                 word[j++] = str[i++];
//             if (str[i] != quote)
//                 return (-1);
//             i++;
//         }
//         else
//             word[j++] = str[i++];
//     }
//     word[j] = '\0';
//     return (0);
// }

// static char *read_word_and_advance(const char *str, int *chars_read, t_quote_flags *quote_flags)
// {
//     int i;
//     int len;
//     char *word;

//     i = 0;
//     len = get_word_length(str, &i, quote_flags);
//     if (len == -1)
//     {
//         *chars_read = i;
//         return ft_strdup("minishell: syntax error: unclosed quotes detected");
//     }
//     word = malloc(sizeof(char) * (len + 1));
//     if (word == NULL)
//         return (NULL);
//     if (write_word_segment(str, word) == -1)
//     {
//         free(word);
//         return ft_strdup("minishell: syntax error: unclosed quotes detected");
//     }
//     *chars_read = i;
//     return (word);
// }

// int add_word_segment(char *str, t_segment **segment_list)
// {
//     int i;
//     t_quote_flags quote_flags;
//     char *word;
//     t_segment *segment;

//     word = read_word_and_advance(str, &i, &quote_flags);
//     if (word == NULL)
//         return (i);
//     segment = create_segment();
//     if (segment != NULL)
//     {
//         if (ft_strncmp(word, "minishell: syntax error", 24) == 0 &&
//             quote_flags.are_quotes_closed == 0)
//             segment->type = ERROR;
//         else
//             segment->type = WORD;
//         segment->str = ft_strdup(word);
//         segment->is_single_quote = quote_flags.in_single_quote;
//         segment->is_double_quote = quote_flags.in_double_quote;
//         segment_add_back(segment_list, segment);
//     }
//     else
//         free(word);
//     free(word);
//     return (i);
// }
