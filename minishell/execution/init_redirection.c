#include "../minishell.h"

static t_segment_type *get_typelist(t_segment *segment,int n, t_data *data, t_cmd *cmd);
static char **get_filename_list(t_segment *segment, t_data *data, t_cmd *cmd);
static int get_n_redir(t_segment *segment_list);
static char **get_arg(t_segment *seg, t_data *data, t_cmd *cmd);

/****************************************************************
Lance l'execution apres avoir separe les arguments des input et output files
******************************************************************/
int init_exec_redir(t_segment *segment_list, t_data *data)
{
	t_cmd		*cmd;
	
	cmd = malloc(sizeof(t_cmd));
	if (!cmd)
		exit_minishell("malloc fail", 1, data);
	cmd->arg = NULL;
	cmd->filename_list = NULL;
	cmd->type_list = NULL;
	cmd->n_redir = get_n_redir(segment_list);
	cmd->arg = get_arg(segment_list, data, cmd);
	cmd->filename_list = get_filename_list(segment_list, data, cmd);
	cmd->type_list = get_typelist(segment_list,cmd->n_redir, data, cmd);
	return (exec_command(cmd, data));
}

static t_segment_type *get_typelist(t_segment *segment,int n, t_data *data, t_cmd *cmd)
{
	t_segment_type *t_list;
	t_segment *cur;
	int i;
	
	t_list = malloc(sizeof(t_segment_type) * n);
	if(!t_list)
		exit_init_exec(cmd, data, "malloc fail");
	i = 0;
	cur = segment;
	while(cur->type != PIPE && cur->type != END_OF_FILE)
	{
		if (is_redirection(cur))
			t_list[i++] = cur->type;
		cur = cur->next;
	}
	return (t_list);
}
static char **get_filename_list(t_segment *segment, t_data *data, t_cmd *cmd)
{
	char **f_list;
	t_segment *cur;

	f_list = NULL;
	cur = segment;
	while(cur->type != PIPE && cur->type != END_OF_FILE)
	{
		if (is_redirection(cur) && cur->next)
			f_list = add_line_cmd(f_list, cur->next->content->str, data, cmd);
		cur = cur->next;
	}
	return (f_list);
	
}

static int get_n_redir(t_segment *segment_list)
{
	t_segment 	*cur;
	int			n;

	n = 0;
	cur = segment_list;
	while(cur->type != PIPE && cur->type != END_OF_FILE)
	{
		if (is_redirection(cur))
			n++;
		cur = cur->next;
	}
	return (n);
}
/****************************************************************
extrait le tableau d'arguments qui sera envoye a l'execution
en ometant les redirection et leurs noms de fichiers
/!\previous du premiers segment doit pointer sur NULL
******************************************************************/
static char **get_arg(t_segment *seg, t_data *data, t_cmd *cmd)
{
	char **arg;
	t_segment *cur;

	arg = NULL;
	cur = seg;
	while(cur->type != PIPE && cur->type != END_OF_FILE)
	{
		if (cur->type == WORD && !is_redirection(cur->previous))
			arg = add_line_cmd(arg, cur->content->str, data, cmd);
		cur = cur->next;
	}
	return (arg);
}