/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leo <leo@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 22:00:39 by mwallis           #+#    #+#             */
/*   Updated: 2025/04/16 18:41:50 by leo              ###   ########.fr       */
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

int g_signal;

int	main(int argc, char **argv, char **envp)
{
	char	*input_line;
	//char	**input_tab;
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
		add_history(input_line);
		segment_list = lexer(input_line, &data);
		//print_segment_list(segment_list);
		free(input_line);
		// faire ici cas specifique ligne vide ou voir si white space
		// Il faudra free inputline probablement ici et plus dans le lexer.
		//a deanoter!
		if (apply_env_expansion(segment_list, &data) == 1)
			continue;
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
		}*/
		data.last_exit_status = init_exec_redir(segment_list, &data);
		free_segment_list(&segment_list);
	}
	return (0);
}