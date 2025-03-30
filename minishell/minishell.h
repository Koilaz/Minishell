/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmarck <lmarck@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 21:58:58 by mwallis           #+#    #+#             */
/*   Updated: 2025/03/30 15:50:57 by lmarck           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

typedef struct s_data
{
	char **env;
	char **env_paths; //A ENLEVER SI NON UTILISER
}	t_data;

typedef enum s_token_type//a definir lors du parsing pour savoir a quoi chaques argument de l'input correspond
{
	WORD,
	PIPE,
	INPUT_REDIRECTION,
	OUTPUT_REDIRECTION,
	APPEND,
	HEREDOC,
	EOF,
	ERROR,
} 	t_token_type;


typedef struct s_token// contient le contenue d'un token sont type et un pointeur sur le token suivant.
{
	char			*str;
	t_token_type	type;
	int             is_simple_quote;
	int             is_double_quote;
	struct s_token	*next;
	struct s_token	*previous;
}	t_token;

# include <stdio.h>          // printf, perror
# include <stdlib.h>         // malloc, free, exit, getenv
# include <unistd.h>         // read, write, close, access, fork, getcwd, chdir, unlink, execve, isatty, ttyname, ...
# include <fcntl.h>          // open
# include <sys/types.h>      // fork, open
//# include <sys/stat.h>       // open, stat, lstat, fstat
//# include <dirent.h>         // opendir, readdir, closedir
//# include <string.h>         // strerror
# include <sys/wait.h>       // wait, waitpid, wait3, wait4
# include <signal.h>
//# include <sys/ioctl.h>      // ioctl
//# include <termios.h>        // tcsetattr, tcgetattr
//# include <curses.h>         // tgetent, tgetflag, tgetnum, tgetstr, tgoto, tputs
//# include <term.h>           // tgetent, tgetflag, tgetnum, tgetstr, tgoto, tputs
# include <readline/readline.h> // readline
# include <readline/history.h>  // add_history
# include <stddef.h>
# include "./libft/libft.h"
# include <limits.h>

# ifndef COLORS_H
#  define COLORS_H

#  define RED "\033[31m" //a mettre en debut de chaine pour lui donner une couleur
#  define GRN "\033[32m"
#  define YEL "\033[33m"
#  define BLU "\033[34m"
#  define MAG "\033[35m"
#  define CYN "\033[36m"
#  define WHT "\033[37m"

#  define RESET "\033[0m"

#  define RED_PROMPT "\001\033[31m\002"
#  define GRN_PROMPT "\001\033[32m\002"
#  define BLU_PROMPT "\001\033[34m\002"

#  define RESET_PROMPT "\001\033[0m\002"

# endif

//Messages d'erreurs :
# define ERROR_ARGC RED "Error: This program does not take any \
arguments." RESET "\n"
# define ERROR_ENV RED "Error: Failed to copy environment \
variables." RESET "\n"
# define W_ID "not a valid identifier\n"
# define MALLOC_FAIL -2


int		init_minishell(char **envp, t_data *data);
void	sigint_handler(int signum);
char	*get_prompt(t_data *data);//donne une string de ce type: "~minishell/minishell coquille >$" ou minishell est le repertoire courant

/*Exit & Error*/

void	mini_exit(int ret, t_data *data);//free puis exit proprement le programme
void	error_exit(char *str, int exit_value, t_data *data);
void	free_data(t_data *data);
void	put_error(char *cmd, char *arg, char *error);

/*Environement*/

int		is_valid_var_name(char *arg);
char	**copy_env(char **envp);
char	*get_env(char *val, char **env);

/*Execution*/

int		built_in_exec(char **arg, t_data *data);//execute les fonctions build in si elles doivent l'etre,
int		is_build_in(char *name);//verifie si un argument est une fonctions built in et renvoie son ID
int		exec_command(char **arg, t_data *data);//prends en argument une commande et ses argument et les executes
void	exec_extern(char **arg, t_data *data);//execute une commande, arg[0] est soit le chemin absolue, relatif ou le nom du programme a cherche dans le PATH de l'environement
int		exec_minishell(char **arg, t_data *data);
void		init_exec_extern(char **arg, t_data *data);

/*Built-in functions*/

/****Exit */

void	bi_exit(char **arg, t_data *data);
int		ft_atoi_with_validation(const char *nptr, int *is_valid);

/****Export*/

int		bi_export(char **arg, t_data *data);
char	**modify_env(char **tab, char *line);
int		var_exist(char **tab, char *line);
void	print_export(char** tab);

/*Utils*/

char	**secure_split(char *line, char sep);
int		ft_is_zero(int n);
char	*ft_str_realoc(char *s1, char const *s2);//Strjoin qui free S1
int		count_line(char **tab);//compte le nombre de lignes d'un tableau nul terminated
char	*add_chr(unsigned char buf, char *str);
void	free_tab(char **tab);//free un tableau de string

#endif
