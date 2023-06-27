/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyunghki <hyunghki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/17 14:17:05 by hyunghki          #+#    #+#             */
/*   Updated: 2023/06/23 18:41:22 by hyunghki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern int	g_status;

static void	ft_parse_heredoc_env(int fd, char **target, t_lst *ev)
{
	t_lst	*ev_val;
	char	tmp;
	int		i;

	i = 0;
	(*target)++;
	while ((*target)[i])
	{
		if (ft_word_chk((*target)[i], "| \t><$\'\"", F_CHK) == 0)
			break ;
		i++;
	}
	tmp = (*target)[i];
	(*target)[i] = '\0';
	ev_val = ft_hash_find(ev, (*target));
	(*target)[i] = tmp;
	while (ev_val != NULL)
	{
		write(fd, ev_val->data, 1);
		ev_val = ev_val->nxt;
	}
	(*target) += i;
}

static void	parse_heredoc(int fd, char *del, int mode, t_lst *ev)
{
	char	*tmp;
	char	*target;

	while (1)
	{
		ft_signal(heredoc_signal_handler, SIG_IGN, 0);
		target = readline("\e[34mheredoc>\e[0m \e[s");
		if (target == NULL)
			printf("\e[u\e[B\e[A");
		if (target == NULL || ft_strcmp(del, target) == 0)
			break ;
		tmp = target;
		while (*tmp)
		{
			if (*tmp == '$' && mode == 1)
				ft_parse_heredoc_env(fd, &tmp, ev);
			else
				write(fd, tmp++, 1);
		}
		write(fd, "\n", 1);
		free(target);
	}
	free(target);
	exit(0);
}

static int	heredoc_parent_wait(int fd, char *file_name)
{
	int	flag;

	ft_signal(SIG_IGN, SIG_IGN, 0);
	waitpid(-1, &flag, 0);
	if (flag != 0)
	{
		close(fd);
		unlink(file_name);
		g_status = 1;
		return (1);
	}
	ft_signal(sigint_handler, SIG_IGN, 0);
	return (0);
}

static int	open_heredoc(char *file_name, t_file *file, t_lst *ev, char *del)
{
	int		fd;
	pid_t	pid;

	del = ft_c_str(file->file_name, NULL, 0, -1);
	if (del == NULL)
		return (ft_error(F_ERROR_MEM));
	fd = open(file_name, O_CREAT | O_RDWR | O_TRUNC, 0644);
	if (fd < 0)
	{
		free(del);
		return (ft_error(F_ERROR_FILE));
	}
	pid = fork();
	if (pid < 0)
	{
		free(del);
		return (ft_error(F_ERROR_FILE));
	}
	else if (pid == 0)
		parse_heredoc(fd, del, ((file->mode & F_NO_TRANS) != 0), ev);
	free(del);
	if (heredoc_parent_wait(fd, file_name) != 0)
		return (1);
	close(fd);
	return (0);
}

int	ft_heredoc(t_file *f, t_lst *ev, char *file_name)
{
	static int	heredoc_num;
	t_lst		*tmp;

	while (1)
	{
		if (heredoc_num == 2147483647)
			return (ft_error(F_ERROR_FILE));
		file_name = ft_itoa(++heredoc_num);
		if (file_name == NULL)
			return (ft_error(F_ERROR_MEM));
		if (access(file_name, F_OK) != 0)
			break ;
		free(file_name);
	}
	tmp = mk_str_lst(file_name);
	if (tmp == NULL || open_heredoc(file_name, f, ev, NULL) != 0)
	{
		free(file_name);
		ft_lst_free(tmp, NULL, F_DATA_CHAR, NULL);
		return (1);
	}
	free(file_name);
	ft_lst_free(f->file_name, NULL, F_DATA_CHAR, NULL);
	f->file_name = tmp;
	return (0);
}
