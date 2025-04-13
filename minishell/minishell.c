/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmarck <lmarck@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 22:00:39 by mwallis           #+#    #+#             */
/*   Updated: 2025/04/13 16:19:34 by lmarck           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Codes d’erreur bash standards :
// 0	Tout va bien (succès)
// 1	Erreur générale / erreur d'exécution
// 2	Mauvais usage d'une commande (ex: syntaxe)
// 126	Commande trouvée mais non exécutable
// 127	Commande introuvable
// 130	Quit via Ctrl+C
// 131	Quit via Ctrl+(\)

// Codes personnels qui n'interfèrent pas avec bash :
// 99 Internal error: NULL input to expand_env_variables
// 98
// 97

int	main(int argc, char **argv, char **envp)
{
	char	*input_line;
	char	**input_tab;
	t_segment	*segment_list;

	t_data	data;
	char *prompt;

	(void)argv;
	if (argc != 1)
		return (write (2, ERROR_ARGC, 60), 1);
	if(minishell_init(envp, &data) == -1)
		return (1);
	while (1)
	{
		prompt = get_prompt(&data);
		if (prompt == NULL)
		{
			write(2, ERROR_PROMPT, 65);
			rl_clear_history();
			free_tab(data.env);
			return (1);
		}
		input_line = readline(prompt);
		free(prompt);
		if (input_line == NULL)
		{
			write(1, "exit\n", 5);
			rl_clear_history();
			free_tab(data.env);
			return (0);
		}
		if (input_line[0] == '\0')
		{
			free(input_line);
			continue ;
		}
		//add_history(input_line);
		//segment_list = lexer(input_line, &data);
		//print_segment_list(segment_list);
		//free(input_line);
		// faire ici cas specifique ligne vide ou voir si white space
		// Il faudra free inputline probablement ici et plus dans le lexer.

		//a deanoter!
		//if (apply_env_expansion(segment_list, &data) == 1)
		//	continue ;
		// printf("\n");
		// print_segment_list(segment_list);

		// continuer ici :
		//--validate_syntax(segment_list);

		// free_segment_list(&segment_list);
		// rl_clear_history();
		// free_tab(data.env);
		//break;

/*------------------------------------------------------*/

/* 		input_tab = ft_split(input_line, ' ');
		if (!input_tab || !input_tab[0])
		{
			free(input_line);
			free_tab(input_tab);
			continue;
		}
		data.last_exit_status = exec_command(input_tab, &data);
		free(input_line);
		free_tab(input_tab);*/

	}


	return (0);
}

/****************************************************************
Lance l'execution apres avoir separe les arguments des input et output files
******************************************************************/
int redir_exec(t_segment *segment_list, t_data *data)
{
	t_segment *current;
	t_redirect *redir_line;
	char **arg;

	arg = get_arg(segment_list, data);
	redir_line = get_redirection(segment_list, data);
}
t_redirect *redir_line(t_segment *segment_list, t_data *data) //Inutile ?!
{
	t_redirect *redir;
	t_segment *cur;
	int i;

	cur = segment_list;
	i = 0;
	while(cur->type != PIPE && cur->type != END_OF_FILE)
	{
		if (is_redirection(cur->type))
			i++;
		cur = cur->next;
	}
	redir = ft_calloc(i + 1, sizeof(t_redirect*));
	if (!redir)
		exit_minishell("malloc failed", MALLOC_FAIL, data);
	cur = segment_list;
	while(cur->type != PIPE && cur->type != END_OF_FILE)
	{
		if (is_redirection(cur->type))
		{

		}
	}
	return (redir);
}

/****************************************************************
extrait le tableau d'arguments qui sera envoye a l'execution
en ometant les redirection et leurs noms de fichiers
/!\previous du premiers segment doit pointer sur NULL
******************************************************************/
char **get_arg(t_segment *seg, t_data *data)
{
	char **arg;
	t_segment *cur;

	cur = seg;
	while(cur->type != PIPE && cur->type != END_OF_FILE)
	{
		if (cur->type == WORD && !is_redirection(cur->previous))
			arg = add_line(arg, cur->content->str, data);
		cur = cur->next;
	}
	return (arg);
}

/****************************************************************
Ajoute une ligne a un tableau en realouant la memoire et free l'ancien
******************************************************************/
char **add_line(char **tab, char *str, t_data *data)
{
	char	**ntab;
	int		i;

	if(!str)
		return (tab);
	i = 0;
	ntab = ft_calloc(count_line(tab) + 2, sizeof(char *));
	if (!ntab)
		exit_minishell("malloc failed", MALLOC_FAIL, data);
	while (tab[i])
	{
		ntab[i] = tab[i];
		i++;
	}
	ntab[i] = ft_strdup(str);
	if (!ntab[i])
	{
		free(tab);
		exit_minishell("malloc failed", MALLOC_FAIL, data);
	}
	free(tab);
	return (ntab);
}
int is_redirection(t_segment *seg)
{
	if(!seg)
		return (0);
	if (seg->type == INPUT_REDIRECTION)
		return (1);
	else if (seg->type == OUTPUT_REDIRECTION)
		return (1);
	else if (seg->type == APPEND)
		return (1);
	else if (seg->type == HEREDOC)
		return (1);
	return (0);
}


/****************************************************************
execute la command de cmd_arg[0] et renvoie le retour de la commande execute
******************************************************************/
int	exec_command(char **cmd_arg, /*t_redirect *redir, */ t_data *data)
{
	if (is_build_in(cmd_arg[0]))
		return (built_in_exec(cmd_arg, data));
	if (ft_strlcmp(cmd_arg[0], "./minishell") == 0)
		return (exec_minishell(cmd_arg, data));
	else
		init_exec_extern(cmd_arg, data);
	return (1);
}
