/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyunghki <hyunghki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/17 14:17:05 by hyunghki          #+#    #+#             */
/*   Updated: 2023/07/04 05:50:42 by hyunghki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern int	g_status;

static void	ft_parse_heredoc_env(int fd, char **to_read, t_lst *ev)
{
	t_lst	*to_find;
	char	*target;
	char	tmp;
	int		i;

	i = 0;
	(*to_read)++;
	while ((*to_read)[i])
	{
		if (ft_str_find("| \t><$\'\"*", (*to_read)[i]) != -1)
			break ;
		i++;
	}
	tmp = (*to_read)[i];
	(*to_read)[i] = '\0';
	to_find = ft_env_find(ev, (*to_read));
	(*to_read)[i] = tmp;
	if (to_find != NULL)
	{
		target = ((char *)to_find->data) + (to_find->info + 1);
		write(fd, target, ft_strlen(target));
	}
	(*to_read) += i;
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
			ft_putstr_fd("\e[u\e[B\e[A", 1);
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
		ft_putstr_fd("\n", fd);
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
		g_status = 1;
		close(fd);
		unlink(file_name);
		free(file_name);
		return (1);
	}
	ft_signal(sigint_handler, SIG_IGN, 0);
	return (0);
}

static int	open_heredoc(char *file_name, char *del, int mode, t_lst *ev)
{
	int		fd;
	pid_t	pid;

	fd = open(file_name, O_CREAT | O_RDWR | O_TRUNC, 0644);
	if (fd < 0)
	{
		free(file_name);
		return (ft_error(F_ERROR_MEM, F_EXIT_STATUS_MEM));
	}
	pid = fork();
	if (pid < 0)
	{
		free(file_name);
		return (ft_error(F_ERROR_MEM, F_EXIT_STATUS_MEM));
	}
	else if (pid == 0)
		parse_heredoc(fd, del, mode, ev);
	if (heredoc_parent_wait(fd, file_name) != 0)
		return (1);
	close(fd);
	return (0);
}

t_lst	*ft_heredoc(char *del, int mode, t_lst *ev)
{
	char	*file_name;
	int		heredoc_num;

	heredoc_num = 0;
	while (heredoc_num < 2147483647)
	{
		file_name = ft_itoa(++heredoc_num, 1);
		if (file_name == NULL)
			return (NULL);
		if (access(file_name, F_OK) != 0)
			break ;
		free(file_name);
	}
	if (heredoc_num == 2147483647)
		return (NULL);
	if (open_heredoc(file_name, del, mode, ev) != 0)
		return (NULL);
	return (mk_lst(file_name, F_DATA_CHAR, F_HEREDOC));
}
