/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exe.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seoklee <seoklee@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/18 13:11:20 by hyunghki          #+#    #+#             */
/*   Updated: 2023/06/18 20:57:52 by seoklee          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	ft_exe_extern(t_lst *path, t_lst *av, char **argv, char **env)
{
	char	*path_char;
	int		i;
	t_lst	*cur;

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
		execve(path_char, argv, env);
		if (cur == NULL)
			path = cur;
		else
			path = cur->nxt;
	}
	exit(ft_error(F_ERROR_EXE));
}

static char	**mk_av(t_lst *data)
{
	char	**target;
	int		i;

	target = ft_calloc(sizeof(char *) * (ft_str_size(data) + 1));
	if (target == NULL)
		return (NULL);
	i = 0;
	while (data != NULL)
	{
		target[i++] = ft_c_str(data->data, NULL, -1, 1);
		if (target[i - 1] == NULL)
			return (ft_av_free(target));
		data = data->nxt;
	}
	return (target);
}

static int	ft_extern(t_lst *data, t_lst *ev, char **env, int is_single)
{
	pid_t	pid;
	t_lst	*path;
	char	**argv;
	int		flag;

	argv = mk_av(data);
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
			ft_exe_extern(path, data->data, argv, env);
		else
			waitpid(-1, &flag, 0);
	}
	else
		ft_exe_extern(path, data->data, argv, env);
	ft_av_free(argv);
	return (flag);
}

static int	ft_exe_cmd(t_token *data, t_lst *ev, char **env, int is_single)
{
	int	flag;
	int	in;
	int	out;

	if (ft_redirection(data, data->redirection) != 0)
		return (1);
	in = dup(0);
	out = dup(1);
	dup2(data->fd[0], 0);
	dup2(data->fd[1], 1);
	flag = 0;
	if (data->argv != NULL)
		flag = ft_built_in_cmd(data->argv, ev);
	if (flag == 2)
		flag = ft_extern(data->argv, ev, env, is_single);
	dup2(in, 0);
	dup2(out, 1);
	close(in);
	close(out);
	return (flag);
}

int	ft_exe(t_lst *tv, t_lst *ev, char **env)
{
	int		is_single;
	int		flag;

	is_single = (tv->nxt == NULL);
	while (tv != NULL)
	{
		flag = ft_exe_cmd(tv->data, ev, env, is_single);
		tv = tv->nxt;
	}
	return (flag);
}
