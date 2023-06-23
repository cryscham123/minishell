/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exe.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyunghki <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/19 13:59:10 by hyunghki          #+#    #+#             */
/*   Updated: 2023/06/23 18:37:22 by hyunghki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern int	g_status;

static void	ft_exe_extern(t_lst *path, t_lst *av, char **argv, t_lst *ev)
{
	char	*path_char;
	int		i;
	t_lst	*cur;

	execve(argv[0], argv, ((t_hash *)ev->data)->env);
	while (path != NULL)
	{
		cur = path;
		i = 0;
		while (cur != NULL && *(char *)cur->data != ':')
		{
			i++;
			cur = cur->nxt;
		}
		path_char = ft_c_str(path, av, i, 0);
		if (path_char == NULL)
			exit(ft_error(F_ERROR_MEM));
		execve(path_char, argv, ((t_hash *)ev->data)->env);
		if (cur == NULL)
			path = cur;
		else
			path = cur->nxt;
		free(path_char);
	}
	exit(ft_error(F_ERROR_EXE));
}

static int	translate_av_ev(t_lst *data, char ***argv, t_lst *ev)
{
	char	**target_av;
	int		i;

	target_av = ft_calloc(sizeof(char *) * (ft_str_size(data) + 1));
	if (target_av == NULL)
		return (1);
	i = 0;
	while (data != NULL)
	{
		target_av[i++] = ft_c_str(data->data, NULL, -1, 1);
		if (target_av[i - 1] == NULL)
		{
			ft_av_free(target_av);
			return (1);
		}
		data = data->nxt;
	}
	if (mk_ev_char(ev) != 0)
	{
		ft_av_free(target_av);
		return (1);
	}
	*argv = target_av;
	return (0);
}

static int	ft_extern(t_lst *data, t_lst *ev, int is_single)
{
	pid_t	pid;
	t_lst	*path;
	char	**argv;
	int		flag;

	if (translate_av_ev(data, &argv, ev) != 0)
		return (1);
	path = ft_hash_find(ev, "PATH");
	flag = 0;
	if (is_single)
	{
		pid = fork();
		if (pid < 0)
			flag = ft_error(F_ERROR_MEM);
		else if (pid == 0)
			ft_exe_extern(path, data->data, argv, ev);
		else
			waitpid(-1, &flag, 0);
	}
	else
		ft_exe_extern(path, data->data, argv, ev);
	ft_av_free(argv);
	return (flag);
}

static int	ft_exe_cmd(t_token *data, t_lst *ev, int is_single, t_lst *prev)
{
	int	flag;
	int	fd_tmp[2];

	if (ft_redirection(data, data->redirection) != 0)
		return (1);
	fd_tmp[0] = dup(0);
	fd_tmp[1] = dup(1);
	dup2(data->fd[0], 0);
	dup2(data->fd[1], 1);
	ft_close(data->fd, prev);
	flag = 0;
	if (data->argv != NULL)
		flag = ft_built_in_cmd(data->argv, ev);
	if (flag == 2)
		flag = ft_extern(data->argv, ev, is_single);
	dup2(fd_tmp[0], 0);
	dup2(fd_tmp[1], 1);
	ft_close(fd_tmp, NULL);
	if (!is_single)
		exit(flag);
	return (flag + 128 * (flag != 0) - 257 * (flag >= 256));
}

int	ft_exe(t_lst *tv, t_lst *ev, t_lst *prev, int i)
{
	pid_t	pid;
	int		is_single;
	int		flag;

	ft_signal(child_signal_handler, child_signal_handler);
	is_single = (tv->nxt == NULL);
	if (is_single)
		return (ft_exe_cmd(tv->data, ev, is_single, tv));
	if (ft_pipe(tv) != 0)
		return (1);
	while (tv != NULL)
	{
		pid = fork();
		if (pid < 0)
			ft_error(F_ERROR_MEM);
		else if (pid == 0)
			ft_exe_cmd(tv->data, ev, is_single, prev);
		if (prev != NULL)
			ft_close(((t_token *)prev->data)->fd, NULL);
		prev = tv;
		tv = tv->nxt;
	}
	while (i--)
		waitpid(-1, &flag, 0);
	return (flag + 128 * (flag != 0) - 257 * (flag >= 256));
}
