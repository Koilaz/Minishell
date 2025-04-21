#include "../minishell.h"

static void set_heredoc(t_cmd *cmd, t_data *data, int i);
static void safe_dup2(t_cmd *cmd, t_data *data, int fd1, int fd2);

int	is_redirection(t_segment *seg)
{
	if (!seg)
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

void	set_redirection(t_cmd *cmd, t_data *data, int i)
{
	int	fd;

	while(++i < cmd->n_redir)
	{
		fd = -1;
		if (cmd->type_list[i] == INPUT_REDIRECTION)
			fd = open(cmd->filename_list[i], O_RDONLY);
		else if (cmd->type_list[i] == OUTPUT_REDIRECTION)
			fd = open(cmd->filename_list[i], O_WRONLY | O_CREAT | O_TRUNC, 0644);
		else if (cmd->type_list[i] == APPEND)
			fd = open(cmd->filename_list[i], O_WRONLY | O_CREAT | O_APPEND, 0644);
		else if (cmd->type_list[i] == HEREDOC)
		{
			set_heredoc(cmd, data, i);
			continue ;
		}
		if (fd < 0)
			exit_init_exec(cmd, data, "open fail");
		if (cmd->type_list[i] == INPUT_REDIRECTION)
			safe_dup2(cmd, data, fd, STDIN_FILENO);
		else if (cmd->type_list[i] == 4 || cmd->type_list[i] == 5)
			safe_dup2(cmd, data, fd, STDOUT_FILENO);
		close(fd);
	}
}
static void set_heredoc(t_cmd *cmd, t_data *data, int i)
{
	int		pipefd[2];
	char	*eof;
	char	*line;

	if (pipe(pipefd) < 0)
		exit_init_exec(cmd, data, "pipe fail");
	eof = cmd->filename_list[i];
	while (42)
	{
		line = readline("> ");
		if (!line || !ft_strlcmp(line, eof))
		{
			if (!line)
				put_error("warning: end-of file", eof,"expected \n");
			free(line);
			break ;
		}
		ft_putstr_fd(line, pipefd[1]);
		write(pipefd[1], "\n", 1);
		free(line);
	}
	close(pipefd[1]);
	safe_dup2(cmd, data, pipefd[0], STDIN_FILENO);
	close(pipefd[0]);
}

void	set_redirection_parent(t_cmd *cmd, t_data *data)
{
	int	bk_stdin;
	int	bk_stdout;

	bk_stdin = dup(STDIN_FILENO);
	bk_stdout = dup(STDOUT_FILENO);
	if (bk_stdin == -1 || bk_stdout == -1)
		exit_init_exec(cmd, data, "dup fail");
	set_redirection(cmd, data, -1);
	data->last_exit_status = built_in_exec(cmd, data);
	if (dup2(bk_stdin, STDIN_FILENO) == -1
		|| dup2(bk_stdout, STDOUT_FILENO) == -1)
		exit_init_exec(cmd, data, "dup2 fail");
}

static void safe_dup2(t_cmd *cmd, t_data *data, int fd1, int fd2)
{
	if (dup2(fd1, fd2) < 0)
		exit_init_exec(cmd, data, "dup2 fail");
}