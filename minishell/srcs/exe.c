/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exe.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyunghki <hyunghki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/19 13:59:10 by hyunghki          #+#    #+#             */
/*   Updated: 2023/06/23 18:41:24 by hyunghki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern int	g_status;

static char	**translate_av_ev(t_lst *data, t_lst *ev, int data_type)
{
	char	**target;
	int		i;

	target = ft_calloc(sizeof(char *) * (ft_str_size(data) + 1));
	if (target == NULL)
		return (ft_lst_free(NULL, target, F_DATA_CHAR, F_ERROR_MEM));
	i = 0;
	while (data != NULL)
	{
		if (data_type == F_DATA_CHAR)
			target[i++] = ft_c_str(data->data, NULL, 0, -1);
		else if (data_type == F_DATA_HASH)
			target[i++] = ft_hash_str(data->data);
		if (target[i - 1] == NULL)
			return (ft_lst_free(NULL, target, F_DATA_CHAR, F_ERROR_MEM));
		data = data->nxt;
	}
	if (data_type == F_DATA_CHAR)
	{
		((t_hash *)ev->data)->env = translate_av_ev(ev->nxt, ev, F_DATA_HASH);
		if (((t_hash *)ev->data)->env == NULL)
			return (ft_lst_free(NULL, target, F_DATA_CHAR, F_ERROR_MEM));
	}
	return (target);
}

static int	ft_extern(t_lst *data, t_lst *ev, int is_single)
{
	pid_t	pid;
	t_lst	*path;
	char	**argv;
	int		flag;

	argv = translate_av_ev(data, ev, F_DATA_CHAR);
	if (argv == NULL)
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
	ft_lst_free(NULL, argv, F_DATA_CHAR, NULL);
	ft_lst_free(NULL, ((t_hash *)ev->data)->env, F_DATA_CHAR, NULL);
	return (flag);
}

static int	ft_exe_cmd(t_token *data, t_lst *ev, int is_single, t_lst *tv)
{
	int	flag;
	int	fd_tmp[2];

	if (ft_redirection(data, data->redirection, is_single) != 0)
		return (1);
	fd_tmp[0] = dup(0);
	fd_tmp[1] = dup(1);
	dup2(data->fd[0], 0);
	dup2(data->fd[1], 1);
	ft_close(data->fd, tv);
	flag = 0;
	if (data->argv != NULL)
		flag = ft_built_in_cmd(data->argv, ev);
	if (flag == F_STATUS_NO_BUILTIN)
		flag = ft_extern(data->argv, ev, is_single);
	dup2(fd_tmp[0], 0);
	dup2(fd_tmp[1], 1);
	ft_close(fd_tmp, NULL);
	if (!is_single)
		exit(flag);
	return (cal_flag(flag));
}

int	ft_exe(t_lst *tv, t_lst *ev, int i)
{
	pid_t	pid;
	int		is_single;
	int		flag;

	ft_signal(SIG_IGN, SIG_IGN, 0);
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
			ft_exe_cmd(tv->data, ev, is_single, tv);
		ft_close(((t_token *)tv->data)->fd, NULL);
		tv = tv->nxt;
	}
	while (i--)
		waitpid(-1, &flag, 0);
	return (cal_flag(flag));
}
