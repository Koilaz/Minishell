/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmarck <lmarck@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 21:58:58 by mwallis           #+#    #+#             */
/*   Updated: 2025/04/12 16:55:03 by lmarck           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

typedef struct s_data
{
	char **env;
	int last_exit_status;
}	t_data;

typedef enum e_quote_type
{
    QUOTE_NONE,
    QUOTE_SINGLE,
    QUOTE_DOUBLE
} t_quote_type;

typedef struct s_word_fragments
{
    char *str;
    t_quote_type quote_type;
    struct s_word_fragments *next;
    struct s_word_fragments *previous;
} t_word_fragments;

typedef enum e_segment_type
{
	SEGMENT_UNSET,
	WORD,
	PIPE,
	INPUT_REDIRECTION,
	OUTPUT_REDIRECTION,
	APPEND,
	HEREDOC,
	END_OF_FILE,
	ERROR_QUOTE
} 	t_segment_type;

typedef struct s_segment
{
	int					index;
	t_word_fragments   	*content;
	t_segment_type		type;
	int            		are_there_quotes_inside;
	struct s_segment	*next;
	struct s_segment	*previous;
}	t_segment;

typedef struct s_redirect {
	int				fd;
	t_segment_type	type;
	char			*filename;
} t_redirect;

# include <unistd.h>         // read, write, close, access, fork, getcwd, chdir, unlink, execve, isatty, ttyname, ...
# include <stdio.h>          // printf, perror
# include <stdlib.h>         // malloc, free, exit, getenv
# include <fcntl.h>          // open
# include <signal.h>
//# include <sys/stat.h>       // open, stat, lstat, fstat
//# include <dirent.h>         // opendir, readdir, closedir
//# include <string.h>         // strerror
//# include <sys/ioctl.h>      // ioctl
//# include <termios.h>        // tcsetattr, tcgetattr
//# include <curses.h>         // tgetent, tgetflag, tgetnum, tgetstr, tgoto, tputs
//# include <term.h>           // tgetent, tgetflag, tgetnum, tgetstr, tgoto, tputs
# include <sys/types.h>      // fork, open
# include <sys/wait.h>       // wait, waitpid, wait3, wait4
# include <readline/readline.h> // readline
# include <readline/history.h>  // add_history
# include <stddef.h>
# include <limits.h>
# include "./libft/libft.h"

#  define RED "\033[31m"
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

# define ERROR_ARGC RED "minishell: this program does not accept \
arguments" RESET "\n"
# define ERROR_ENV RED "minishell: failed to initialize environment: \
memory exhausted" RESET "\n"
# define ERROR_PROMPT RED "minishell: failed to generate prompt: \
memory exhausted" RESET "\n"
# define ERROR_LEXER RED "minishell: unable to tokenize input \
(lexer): memory exhausted" RESET "\n"
# define ERROR_INTERNAL_NULL_EXPAND RED "minishell: internal \
error: unexpected NULL in expand_env_variables" RESET "\n"
# define ERROR_EXPAND_MALLOC RED "minishell: failed to expand \
environment variable: memory exhausted" RESET "\n"

# define ERROR_MISSING_BRACE "minishell: bad substitution: \
missing `}'\n"
# define ERROR_PID_EXPAND "minishell: failed to retrieve \
PID: not supported on this system\n"



# define ERROR_ARG "Error: This function does not take any \
arguments.\n"
# define ERROR_OPTION "Error: invalid option\n"
# define W_ID "not a valid identifier\n"

//fail return value :
# define MALLOC_FAIL 1
# define GETCWD_FAIL 1
# define WRITE_FAIL 1
# define FORK_FAIL 1



t_segment   *lexer(char *input_line, t_data *data);
void	print_segment_list(t_segment *list);
void	free_segment_list(t_segment **segment);
int    apply_env_expansion(t_segment	*segment_list, t_data *data);
int minishell_init(char **envp, t_data *data);












void	sigint_handler(int signum);
char	*get_prompt(t_data *data);//donne une string de ce type: "~minishell/minishell coquille >$" ou minishell est le repertoire courant

/*Exit & Error*/

void	mini_exit(int ret, t_data *data);//free puis exit proprement le programme
void	exit_minishell(char *str, int exit_value, t_data *data);
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
void	init_exec_extern(char **arg, t_data *data);

/*Built-in functions*/

/****Exit */

void	bi_exit(char **arg, t_data *data);
int		ft_atoi_with_validation(const char *nptr, int *is_valid);

/****Export*/

int		bi_export(char **arg, t_data *data);
char	**modify_env(char **tab, char *line, t_data *data);
int		var_exist(char **tab, char *line, t_data *data);
void	print_export(char** tab);

/****Echo*/

int		bi_echo(char **arg, int i, t_data *data);
int		ft_putstrs_fd(char *s, int fd);

/****unset */

char **delete_line(int line, char **tab, t_data *data);
int find_index(char *arg, char **env);
int bi_unset(char **arg, t_data *data);

/****env */

int bi_env(char **arg, t_data *data);

/****pwd */

int bi_pwd(char **arg, t_data *data);

/****cd */

int bi_cd(char **arg, t_data *data);
int cd_error(char *arg, char *per);
char *str_to_export(char *name, char *val);
char *cd_path(char **arg, t_data *data);

/*Utils*/

char	**secure_split(char *line, char sep, t_data *data);
int		ft_is_zero(int n);
char	*ft_str_realoc(char *s1, char const *s2);//Strjoin qui free S1
int		count_line(char **tab);//compte le nombre de lignes d'un tableau nul terminated
char	*add_chr(unsigned char buf, char *str);
void	free_tab(char **tab);//free un tableau de string
int		ft_strlcmp(const char *s1, const char *s2);

#endif
