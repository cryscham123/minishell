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

static int	heredoc_parent_wait(int fd, t_lst *file)
{
	int	flag;

	ft_signal(SIG_IGN, SIG_IGN, 0);
	waitpid(-1, &flag, 0);
	if (flag != 0)
	{
		close(fd);
		ft_unlink(file);
		ft_lst_free(file, NULL, F_DATA_CHAR, NULL);
		g_status = 1;
		return (1);
	}
	ft_signal(sigint_handler, SIG_IGN, 0);
	return (0);
}

static t_lst	*create_heredoc(char *del, int mode, int token_num, t_lst *ev)
{
	int		fd;
	t_lst	*file;
	char	*tmp;
	pid_t	pid;

	file = ft_itoa(token_num);
	if (file == NULL)
		return (ft_lst_free(file, NULL, F_DATA_CHAR, F_ERROR_MEM));
	tmp = ft_c_str(file, NULL, 0, -1);
	if (tmp == NULL)
		return (ft_lst_free(file, NULL, F_DATA_CHAR, F_ERROR_MEM));
	fd = open(tmp, O_CREAT | O_RDWR | O_TRUNC, 0644);
	free(tmp);
	if (fd < 0)
		return (ft_lst_free(file, NULL, F_DATA_CHAR, F_ERROR_FILE));
	pid = fork();
	if (pid < 0)
		return (ft_lst_free(file, NULL, F_DATA_CHAR, F_ERROR_FILE));
	else if (pid == 0)
		parse_heredoc(fd, del, mode, ev);
	else if (heredoc_parent_wait(fd, file) != 0)
		return (NULL);
	close(fd);
	return (file);
}

int	ft_heredoc(t_file *f, int token_num, t_lst *ev)
{
	char	*del;
	t_lst	*tmp;

	del = ft_c_str(f->file_name, NULL, 0, -1);
	if (del == NULL)
		return (ft_error(F_ERROR_MEM));
	tmp = create_heredoc(del, ((f->mode & F_NO_TRANS) == 0), token_num, ev);
	f->mode &= ~F_NO_TRANS;
	free(del);
	if (tmp == NULL)
		return (1);
	ft_lst_free(f->file_name, NULL, F_DATA_CHAR, NULL);
	f->file_name = tmp;
	return (0);
}
