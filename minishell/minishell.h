/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leo <leo@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 21:58:58 by mwallis           #+#    #+#             */
/*   Updated: 2025/04/21 12:19:29 by leo              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

extern int	g_signal;

typedef struct s_data
{
	char	**env;
	int		last_exit_status;
}	t_data;

typedef enum e_quote_type
{
	QUOTE_NONE,
	QUOTE_SINGLE,
	QUOTE_DOUBLE
}	t_quote_type;

typedef struct s_word_fragments
{
	char					*str;
	t_quote_type			quote_type;
	struct s_word_fragments	*next;
	struct s_word_fragments	*previous;
}	t_word_fragments;

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
}	t_segment_type;

typedef struct s_segment
{
	int					index;
	t_word_fragments	*content;
	t_segment_type		type;
	int					are_there_quotes_inside;
	struct s_segment	*next;
	struct s_segment	*previous;
}	t_segment;

typedef struct s_lexer_norm_helper
{
	int				i;
	int				start;
	int				len;
	t_quote_type	quote_type;
}	t_lexer_norm_helper;

typedef struct s_cmd {
	int				n_redir;
	char 			**filename_list;
	t_segment_type	*type_list;
	char			**arg;
} t_cmd;

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

# define DEFAULT_PATH "PATH=/usr/local/sbin:/usr/local/bin:" \
					  "/usr/sbin:/usr/bin:/sbin:/bin"
# define ERROR_SHLVL_INVALID "minishell: warning: shell level \
(SHLVL) invalid, resetting to 1\n"
# define ERROR_SHLVL_TOO_HIGH "minishell: warning: shell level \
(SHLVL) too high or invalid, resetting to 1\n"

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

// Temporaires max
int    apply_env_expansion(t_segment	*segment_list, t_data *data);


/* ********************************************************************** */
/*                                Parsing                                 */
/* ********************************************************************** */


// --------------------------- Initialization --------------------------- //

int		minishell_init(char **envp, t_data *data);
char	**init_minimal_env(void);
char	**complete_env_if_needed(char **env);
char	*increase_shlvl(char *old_shell_lvl);
void	sigint_handler(int signum);

// ------------------------------- Prompt ------------------------------- //

char	*get_prompt(t_data *data);
char	*get_host(void);

// ------------------------------- Lexer -------------------------------- //

t_segment	*lexer(char *input_line, t_data *data);
int			add_word_segment(char *str, t_segment **segment_list, char *origin, t_data *data);
t_word_fragments	*split_word_into_fragments(const char *str, int *chars_read, int *are_quotes_closed);
int			add_operator_segment(char *str, t_segment **segment_list, char *origin, t_data *data);
t_segment	*create_segment(void);
void		segment_add_back(t_segment **segment_list, t_segment *new_segment);
t_word_fragments   *create_fragment(char *str, t_quote_type quote_type);
void		fragment_add_back(t_word_fragments **fragments_list, t_word_fragments *new_fragment);
void		free_segment_list(t_segment **segment);
void		free_word_fragments(t_word_fragments *fragment);
int			skip_white_space(char *str);
int			is_separator(char c);
void		lexer_error_exit(char *origin, t_segment **segment_list, t_data *data);

// Supprimer ou annoter a la fin :
//void print_segment_list(t_segment *segment_list);

// ----------------------------- Expansion ------------------------------ //

/*Exit & Error*/

void	mini_exit(int ret, t_data *data);//free puis exit proprement le programme
void	exit_minishell(char *str, int exit_value, t_data *data);
void	free_data(t_data *data);
void	put_error(char *cmd, char *arg, char *error);
void 	exit_init_exec(t_cmd *cmd, t_data *data, char *str);

/*Environement*/

int		is_valid_var_name(char *arg);
char	*get_env(char *val, char **env);

/*Execution*/

int		built_in_exec(t_cmd *cmd, t_data *data);//execute les fonctions build in si elles doivent l'etre,
int		is_build_in(char *name);//verifie si un argument est une fonctions built in et renvoie son ID
int		exec_command(t_cmd *cmd, t_data *data);//prends en argument une commande et ses argument et les executes
void	exec_extern(t_cmd *cmd, t_data *data);//execute une commande, arg[0] est soit le chemin absolue, relatif ou le nom du programme a cherche dans le PATH de l'environement
int		exec_minishell(t_cmd *cmd, t_data *data);
void	init_exec_extern(t_cmd *cmd, t_data *data);
char	**add_line_cmd(char **tab, char *str, t_data *data, t_cmd *cmd);
void 	free_cmd(t_cmd *cmd);

/*redirection*/

int		init_exec_redir(t_segment *segment_list, t_data *data);
int		is_redirection(t_segment *seg);
void	set_redirection(t_cmd *cmd, t_data *data, int i);
void 	set_redirection_parent(t_cmd *cmd, t_data *data);

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
